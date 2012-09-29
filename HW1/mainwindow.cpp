#include "mainwindow.h"
#include <QTableWidgetItem>
#include <QLabel>
#include <QLayout>
#include <QAction>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QProgressBar>
#include <QDebug>

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
    QList<QTableWidgetItem*> result = myMainTable->findItems(qUrl.toString(), 0);
    if(result.count() != 0) {
        setStatus("There is file with same url: " + 
                  qUrl.toString());
        return;
    }
    
    myMainTable->setRowCount(myMainTable->rowCount() + 1);
    myMainTable->setSortingEnabled(false);
    
    int itemLineNum = myMainTable->rowCount() - 1;
    int itemNumber = ++myMaxNumber;
    
    QTableWidgetItem *cellItemNumber = new QTableWidgetItem;
    cellItemNumber->setData(Qt::DisplayRole, itemNumber);
    myMainTable->setItem(itemLineNum, 0, cellItemNumber);
    
    QTableWidgetItem *cellItemUrl = new QTableWidgetItem(
                qUrl.toString());
    myMainTable->setItem(itemLineNum, 1, cellItemUrl);
    
    QTableWidgetItem *cellItemLocation = new QTableWidgetItem(location);
    myMainTable->setItem(itemLineNum, 2, cellItemLocation);
    
    QTableWidgetItem *cellItemProgress = new QTableWidgetItem;
    cellItemProgress->setData(Qt::DisplayRole, 0);
    myMainTable->setItem(itemLineNum, 3, cellItemProgress);
    
    QProgressBar* qpb = new QProgressBar(this);
    qpb->setMaximum(100);
    qpb->setValue(0);
    myMainTable->setCellWidget(itemLineNum, 3, qpb);
    
    myMainTable->setSortingEnabled(true);
    
    setStatus("file " + qUrl.toString() + " begin downloading");

    doDownload(qUrl);

}

void MainWindow::removeLoadItem(QString url) {
    
    QList<QTableWidgetItem*> result = myMainTable->findItems(url, 0);
    myMainTable->removeRow(result[0]->row());
    
}

void MainWindow::doDownload(const QUrl &url) {
    QNetworkRequest request(url);
    QNetworkReply *reply = manager->get(request);    
    currentDownloads.append(reply);
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




















