#ifndef READNAME_H
#define READNAME_H

#include <QDialog>
#include<string>

using  std::string;

namespace Ui {
class readname;
}

class readname : public QDialog
{
    Q_OBJECT

public:
    explicit readname(QWidget *parent = 0);
    ~readname();


signals:
    void sendData(QString);


private slots:
    void on_pushButton_clicked();

private:
    Ui::readname *ui;
    void save();

};

#endif // READNAME_H
