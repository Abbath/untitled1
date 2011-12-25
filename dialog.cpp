#include "dialog.h"
#include "ui_dialog.h"

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
}

Dialog::~Dialog()
{

    delete ui;
}
char *Dialog::text(){
    QString a = ui->plainTextEdit->toPlainText();
    QByteArray b = a.toLocal8Bit();
    return b.data();
}
void Dialog::clear(){
    ui->plainTextEdit->clear();

}
