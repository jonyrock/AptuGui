#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui/QMainWindow>
#include <QStatusBar>
#include <QMenuBar>
#include <QTableWidget>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>

#include "dialodadditem.h"

class MainWindow : public QMainWindow{
    
    Q_OBJECT
    QNetworkAccessManager* manager;
    
public:
    MainWindow(QWidget *parent = 0);

private slots:
    void openAddItemDialog();
    void downloadProgress(qint64 bytesReceived, qint64 bytesTotal);
    void downloadFinished(QNetworkReply *reply);
    
private:
    
    QTableWidget* myMainTable;
    
    int myMaxNumber;
    QWidget* myStatusWidget;
    
    void addLoadItem(QString url, QString location);
    void removeLoadItem(QString url);
    void initMainTable();
    void setStatus(QString value);
    void doDownload(const QUrl &url);
    int findRowByUrl(QString url);
    
    
    
};

#endif // MAINWINDOW_H
