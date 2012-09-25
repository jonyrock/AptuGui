#ifndef DIALODADDITEM_H
#define DIALODADDITEM_H

#include <QDialog>
#include <QLineEdit>

class DialodAddItem : public QDialog
{
    Q_OBJECT
public:
    explicit DialodAddItem(QWidget *parent = 0);
    
    QString url();
    QString location();

private:
    QLineEdit* myUrlEdit;
    QLineEdit* myLocationEdit;

};

#endif // DIALODADDITEM_H
