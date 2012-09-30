#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui/QMainWindow>
#include <QStatusBar>
#include <QMenuBar>
#include <QTableWidget>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>
#include <QMap>

#include "dialodadditem.h"

class MainWindow : public QMainWindow{
    
    Q_OBJECT
    QNetworkAccessManager* manager;
    QMap<QString, QNetworkRequest> mapUrlToRequest;
    
public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void openAddItemDialog();
    void downloadProgress(qint64 bytesReceived, qint64 bytesTotal);
    void downloadFinished(QNetworkReply *reply);
    void tableClick(int, int);
    
    
    
private:
    
    void loadData();
    void saveData();
    
    QTableWidget* myMainTable;
    
    int myMaxNumber;
    QWidget* myStatusWidget;
    QIcon myIcoLow;
    QIcon myIcoNormal;
    QIcon myIcoHigh;
    
    void addLoadItem(QString url, QString location);
    void removeLoadItem(QString url);
    void initMainTable();
    void setStatus(QString value);
    void doDownload(const QUrl &url);
    int findRowByUrl(QString url);
    bool checkSaveLocation(QString location);
    void saveFile(QString location, const QByteArray& data);
    
    
    
};

#endif // MAINWINDOW_H
