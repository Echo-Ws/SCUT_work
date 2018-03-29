#ifndef RECORD_H
#define RECORD_H

#include <QDialog>
#include"readname.h"



namespace Ui {
class record;
}

class record : public QDialog
{
    Q_OBJECT

public:
    explicit record(QWidget *parent = 0);
    ~record();


public slots:
    void can_save(int);
    void receiveData(QString );
    void show();





private slots:
    void on_pushButton_2_clicked();

    void on_pushButton_clicked();

private:
    Ui::record *ui;

    int sco[3];
    QString nam[3];

    void getname();
   QString player;


    void loadrecord();
    void save();
};

#endif // RECORD_H
