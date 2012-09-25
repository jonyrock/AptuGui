#include "mainwindow.h"
#include <QTableWidgetItem>
#include <QLabel>
#include <QLayout>
#include <QAction>
#include <QVBoxLayout>
#include <QHBoxLayout>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), myMaxNumber(0), myStatusWidget(NULL) {
    
    this->setMinimumWidth(600);
    this->setMinimumHeight(400);
    
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
    
    myMainTable->setRowCount(myMainTable->rowCount() + 1);
    
    int itemLineNum = myMainTable->rowCount() - 1;
    int itemNumber = ++myMaxNumber;
    
    QTableWidgetItem *cellItemNumber = new QTableWidgetItem(
                QString::number(itemNumber));
    myMainTable->setItem(itemLineNum, 0, cellItemNumber);
    
    QTableWidgetItem *cellItemUrl = new QTableWidgetItem(url);
    myMainTable->setItem(itemLineNum, 1, cellItemUrl);
    
    QTableWidgetItem *cellItemLocation = new QTableWidgetItem(location);
    myMainTable->setItem(itemLineNum, 2, cellItemLocation);
    
}


void MainWindow::openAddItemDialog() {
    DialodAddItem* d = new DialodAddItem();
    if(d->exec() == QDialog::Accepted) {
        addLoadItem(d->url(), d->location());
    }
}

MainWindow::~MainWindow() {}

void MainWindow::setStatus(QString value) {
    if(myStatusWidget != NULL) {
        delete myStatusWidget;
        myStatusWidget = NULL;
    }
    myStatusWidget = new QLabel(value);
    statusBar()->addWidget(myStatusWidget);
}





















