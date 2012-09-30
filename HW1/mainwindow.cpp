#include "mainwindow.h"
#include <QTableWidgetItem>
#include <QLabel>
#include <QLayout>
#include <QAction>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QProgressBar>
#include <QDebug>
#include <QFile>

#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), myMaxNumber(0), myStatusWidget(NULL) {
    
    this->setMinimumWidth(600);
    this->setMinimumHeight(400);
    
    manager = new QNetworkAccessManager(this);
    
    
    QHBoxLayout* mainLayout= new QHBoxLayout(this);
    
    QWidget *window = new QWidget();
    window->setLayout(mainLayout);
    setCentralWidget(window);
            
    /* Init widgets */
    initMainTable();
    mainLayout->addWidget(myMainTable);
    
    this->setWindowTitle("Downloader");
    
    QAction* addItemAction = new QAction("Add file", this);
    connect(addItemAction, SIGNAL(triggered()), this, 
            SLOT(openAddItemDialog()));
    connect(manager, SIGNAL(finished(QNetworkReply*)),
                 SLOT(downloadFinished(QNetworkReply*)));
    

    QMenu* mainMenu = new QMenu();
    mainMenu->setTitle("File");
    mainMenu->addAction(addItemAction);
    menuBar()->addMenu(mainMenu);
    
    myIcoLow = QIcon(":/icons/p1");
    myIcoNormal = QIcon(":/icons/p2");
    myIcoHigh = QIcon(":/icons/p3");
    
    loadData();
        
}

void MainWindow::initMainTable() {
    myMainTable = new QTableWidget(0,5,this);
    myMainTable->setHorizontalHeaderItem(0, new QTableWidgetItem("#"));
    myMainTable->setHorizontalHeaderItem(1, new QTableWidgetItem("Name"));
    myMainTable->setHorizontalHeaderItem(2, new QTableWidgetItem("Location"));
    myMainTable->setHorizontalHeaderItem(3, new QTableWidgetItem("Percent"));
    myMainTable->setHorizontalHeaderItem(4, new QTableWidgetItem("Priority"));
    myMainTable->setShowGrid(false);
    myMainTable->setColumnWidth(0, 50);
    myMainTable->setSortingEnabled(true);
    connect(myMainTable, SIGNAL(cellClicked(int,int)), 
            this, SLOT(tableClick(int, int)));
}

void MainWindow::addLoadItem(QString url, QString location) {
    
    if(url.isEmpty()) {
        setStatus("ERROR: Wrong url");
        return;
    }
    
    if(location.isEmpty()) {
        setStatus("ERROR: Wrong location");
        return;
    }
    QUrl qUrl = QUrl::fromUserInput(url);
    if(findRowByUrl(qUrl.toString()) != -1) {
        setStatus("There is file with same url: " + 
                  qUrl.toString());
        return;
    }
    
    if(!checkSaveLocation(location)){
        setStatus("Can't use location for saving: " + location);
        return;
    }
    
    myMainTable->setRowCount(myMainTable->rowCount() + 1);
    myMainTable->setSortingEnabled(false);
    
    int itemLineNum = myMainTable->rowCount() - 1;
    int itemNumber = ++myMaxNumber;
    
    QTableWidgetItem *cellItemNumber = new QTableWidgetItem;
    cellItemNumber->setData(Qt::DisplayRole, itemNumber);
    myMainTable->setItem(itemLineNum, 0, cellItemNumber);
    
    QTableWidgetItem *cellItemUrl = new QTableWidgetItem;
    cellItemUrl->setText(qUrl.toString());
    myMainTable->setItem(itemLineNum, 1, cellItemUrl);
    
    QTableWidgetItem *cellItemLocation = new QTableWidgetItem;
    cellItemLocation->setText(location);
    myMainTable->setItem(itemLineNum, 2, cellItemLocation);
    
    QTableWidgetItem *cellItemProgress = new QTableWidgetItem;
    cellItemProgress->setData(Qt::DisplayRole, 0);
    myMainTable->setItem(itemLineNum, 3, cellItemProgress);
        
    QProgressBar* qpb = new QProgressBar(this);
    qpb->setMaximum(100);
    qpb->setValue(0);
    myMainTable->setCellWidget(itemLineNum, 3, qpb);
    
    QTableWidgetItem *cellItemPriority = new QTableWidgetItem;
    cellItemPriority->setIcon(myIcoNormal);
    cellItemPriority->setText("Normal");
    myMainTable->setItem(itemLineNum, 4, cellItemPriority);
    
    setStatus("file " + qUrl.toString() + " begin downloading");
    
    doDownload(qUrl);

}

void MainWindow::removeLoadItem(QString url) {
    
    int row = findRowByUrl(url);
    myMainTable->removeRow(row);
    mapUrlToRequest.remove(url);
    
}

void MainWindow::doDownload(const QUrl &url) {
    
    QNetworkRequest request(url);
    mapUrlToRequest.insert(url.toString(), request);
    request.setPriority(QNetworkRequest::LowPriority);
    QNetworkReply *reply = manager->get(request);
    connect(reply, SIGNAL(downloadProgress(qint64,qint64)),
            this, SLOT(downloadProgress(qint64, qint64))
    );
}

void MainWindow::downloadProgress(qint64 bytesReceived, 
                                  qint64 bytesTotal) {
    
    if(bytesTotal == 0) return;
    
    QNetworkReply* reply = static_cast<QNetworkReply*>(
                QObject::sender());
    int percent = (bytesReceived * 100) / bytesTotal;
    int itemLineNum = findRowByUrl(reply->url().toString());
    
    QTableWidgetItem *cellItemProgress = 
            myMainTable->item(itemLineNum, 3);
    cellItemProgress->setData(Qt::DisplayRole, percent);
    
    
    QProgressBar* qpb = static_cast<QProgressBar*>(
                myMainTable->cellWidget(itemLineNum, 3));
    qpb->setValue(percent);
    
            
}

void MainWindow::downloadFinished(QNetworkReply *reply)
 {
     QUrl url = reply->url();
     if (reply->error()) {
         
        setStatus( 
            "Download of " +
            url.toEncoded() + " failed: " + 
            qPrintable(reply->errorString())
        );
        removeLoadItem(reply->url().toString());
        return;
                 
     } else {
        setStatus(
            "Download of " + url.toEncoded() + " is finished."
        );
        int row = findRowByUrl(reply->url().toString());
        QTableWidgetItem * item = myMainTable->item(row, 2);
        QString location = item->text();
        saveFile(location, reply->readAll());
        removeLoadItem(reply->url().toString());
        return;
     }

 }

void MainWindow::openAddItemDialog() {
    DialodAddItem* d = new DialodAddItem();
    if(d->exec() == QDialog::Accepted) {
        addLoadItem(d->url(), d->location());
    }
}

void MainWindow::setStatus(QString value) {
    if(myStatusWidget != NULL) {
        delete myStatusWidget;
        myStatusWidget = NULL;
    }
    myStatusWidget = new QLabel(value);
    statusBar()->addWidget(myStatusWidget);
}

int MainWindow::findRowByUrl(QString url) {
    
    QList<QTableWidgetItem*> result = 
            myMainTable->findItems(url, 0);
    if(result.count() == 0) return -1;
    return result[0]->row();
    
}

bool MainWindow::checkSaveLocation(QString location) {              
    QFile file(location);
    bool res = file.open(QIODevice::WriteOnly);
    file.close();
    return res;
}

void MainWindow::saveFile(QString location, const QByteArray& data) {
    QFile file(location);
    file.open(QIODevice::WriteOnly);
    file.write(data);
    file.close();
}

void MainWindow::tableClick(int row, int column) {
    if(column != 4) return;
    QString currentText = myMainTable->item(row, 4)->text();
    QNetworkRequest::Priority newPriority;
    QString newText;
    QIcon newIco;
    if(currentText == "Normal") {
        newPriority = QNetworkRequest::HighPriority;
        newText = "High";
        newIco = myIcoHigh;
    }
    if(currentText == "High") {
        newPriority = QNetworkRequest::LowPriority;
        newText = "Low";
        newIco = myIcoLow;
    }
    if(currentText == "Low") {
        newPriority = QNetworkRequest::NormalPriority;
        newText = "Normal";
        newIco = myIcoNormal;
    }
    QString url = myMainTable->item(row, 1)->text();
    mapUrlToRequest.find(url)->setPriority(newPriority);
    myMainTable->item(row, column)->setText(newText);
    myMainTable->item(row, column)->setIcon(newIco);
}

void MainWindow::saveData() {
    
    QFile file("data.dat");
    file.open(QIODevice::WriteOnly);
    
    QTextStream outStream(&file);
    
    for(int i = 0; i < myMainTable->rowCount(); i++) {
        QString url = myMainTable->takeItem(i,1)->text();
        QString location = myMainTable->takeItem(i,2)->text();
        QString priority = myMainTable->takeItem(i,4)->text();
        outStream << url << "|&|" << location 
                  << "|&|" << priority << "|*|";
    }
    file.close();
    
}

void MainWindow::loadData() {
    
    if(!QFile::exists("data.dat")) return;
    QFile file("data.dat");
    if(!file.open(QIODevice::ReadOnly)) return;
    QTextStream stream (&file);
    QString str = stream.readLine();
    QStringList list = str.split("|*|");
    for(int i = 0; i < list.length(); i++) {
        QStringList values = list[i].split("|&|");
        if(values.length() < 3) continue;
        addLoadItem(values[0], values[1]);
        int newRow = findRowByUrl(values[0]);
        if(values[2] == "High") {
            tableClick(newRow, 4);
        }
        if(values[2] == "Low") {
            tableClick(newRow, 4);
            tableClick(newRow, 4);
        }
    }
    file.close();
    
}

MainWindow::~MainWindow(){
    saveData();
}













