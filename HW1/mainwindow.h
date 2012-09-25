#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui/QMainWindow>
#include <QStatusBar>
#include <QMenuBar>
#include <QTableWidget>

#include "dialodadditem.h"

class MainWindow : public QMainWindow{
    
    Q_OBJECT
    
public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void openAddItemDialog();
    
private:
    
    QTableWidget* myMainTable;
    
    int myMaxNumber;
    QWidget* myStatusWidget;
    
    void addLoadItem(QString url, QString location);
    void initMainTable();
    void setStatus(QString value);
    
};

#endif // MAINWINDOW_H
