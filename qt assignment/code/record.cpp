#include "record.h"
#include "ui_record.h"



#include<QString>


#include<QFile>
#include<QTextStream>

#include<QDebug>

using std::fstream;
using std::ios;


using  std::string;



record::record(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::record)
{
    ui->setupUi(this);

    nam[0]=nam[1]=nam[2]="UnknowPlayer";
    sco[0]=sco[1]=sco[2]=0;
    loadrecord();

}


record::~record()
{
    delete ui;
}



void record::show()
{
    loadrecord();

    ui->name_1->setText(nam[0]);
    ui->score_1->setText(QString::number(sco[0]));

    ui->name_2->setText(nam[1]);
    ui->score_2->setText(QString::number(sco[1]));

    ui->name_3->setText(nam[2]);
    ui->score_3->setText(QString::number(sco[2]));

    QDialog::show();
    this->exec();//防止响应其他窗口
}


void record::can_save(int score)
{
    int f=-1;
    for(int i=0;i<3;i++)
    {
        if(score>=sco[i])
        {
            f=i;
            break;
        }
    }


    if(0<=f&&f<=2)
    {
        getname();
        switch(f)//根据大小排位选择保存位置
        {
        case 0:
            sco[2]=sco[1];
            sco[1]=sco[0];
            sco[0]=score;
            nam[2]=nam[1];
            nam[1]=nam[0];
            nam[0]=player;
            break;
        case 1:
            sco[2]=sco[1];
            sco[1]=score;
            nam[2]=nam[1];
            nam[1]=player;
            break;
        case 2:
            sco[2]=score;
            nam[2]=player;
            break;
            default:break;
        }
        save();

        qDebug()<<player;//检测是否正常保存

        show();
    }


 }

void record::getname()
{
    readname *rname;//connect参数是Qobject 所以必须用指针
    rname=new readname();
    connect(rname,SIGNAL(sendData(QString)),this,SLOT(receiveData(QString)));


    rname->exec();

}



void record::receiveData(QString rec)
{
    player=rec;


}


void record::save()
{
    QFile file1("save.txt");

    QTextStream input(&file1);

   file1.open(QIODevice::WriteOnly|QIODevice::Text);


       for(int i=0;i<3;i++)
       {
           if(nam[i].length()==0)
               nam[i]="UnknowPlayer";
           input<<nam[i]<<"\n "<<sco[i]<<"\n";

       }
        file1.close();



}



void record::loadrecord()
{
    QFile file1("save.txt");

    QTextStream output(&file1);

    file1.open(QIODevice::ReadOnly|QIODevice::Text);




           for(int i=0;i<3;i++)
           {
                nam[i]=output.readLine();
                sco[i]=output.readLine().toInt();

           }


        file1.close();




}




void record::on_pushButton_2_clicked()
{
    accept();
}

void record::on_pushButton_clicked()
{
    nam[0]=nam[1]=nam[2]="UnknowPlayer";
    sco[0]=sco[1]=sco[2]=0;
    save();
    ui->name_1->setText(nam[0]);
    ui->score_1->setText(QString::number(sco[0]));
    ui->name_2->setText(nam[1]);
    ui->score_2->setText(QString::number(sco[1]));
    ui->name_3->setText(nam[2]);
    ui->score_3->setText(QString::number(sco[2]));
}
