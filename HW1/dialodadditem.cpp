#include "dialodadditem.h"
#include <QFormLayout>
#include <QLabel>
#include <QPushButton>

DialodAddItem::DialodAddItem(QWidget *parent) :
    QDialog(parent){
    
    myUrlEdit = new QLineEdit;
    myLocationEdit = new QLineEdit;
    
    this->setWindowTitle("Download new file");
    this->setFixedSize(300, 100);
    QFormLayout* qg = new QFormLayout();
    qg->setMargin(10);
    qg->addRow("Url:", myUrlEdit);
    qg->addRow("Location:", myLocationEdit);
    this->setLayout(qg);
    
    QPushButton* okButton = new QPushButton(this);
    okButton->setText("OK");
    qg->addRow(okButton);
    
    connect(okButton, SIGNAL(clicked()), this, SLOT(accept()));
    
}

QString DialodAddItem::url() {
    return myUrlEdit->text();
}

QString DialodAddItem::location() {
    return myLocationEdit->text();
}


