#include "dialogadditem.h"
#include "ui_dialogadditem.h"

DialogAddItem::DialogAddItem(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogAddItem)
{
    ui->setupUi(this);
}

DialogAddItem::~DialogAddItem()
{
    delete ui;
}
