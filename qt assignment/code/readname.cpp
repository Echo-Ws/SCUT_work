#include "readname.h"
#include "ui_readname.h"
#include <QRegExp>


readname::readname(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::readname)
{
    ui->setupUi(this);

    //QRegExp regExp("[/w*]");
   // ui->lineEdit->setValidator(new QRegExpValidator(regExp, this));
}

readname::~readname()
{
    delete ui;
}

void readname::on_pushButton_clicked()
{
    emit sendData(ui->lineEdit->text());
    accept();
}
