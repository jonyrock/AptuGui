#ifndef DIALOGADDITEM_H
#define DIALOGADDITEM_H

#include <QDialog>

namespace Ui {
    class DialogAddItem;
}

class DialogAddItem : public QDialog
{
    Q_OBJECT

public:
    explicit DialogAddItem(QWidget *parent = 0);
    ~DialogAddItem();

private:
    Ui::DialogAddItem *ui;
};

#endif // DIALOGADDITEM_H
