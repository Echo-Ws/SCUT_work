
#include <QApplication>
#include"myview.h"


int main(int argc, char *argv[])
{
    qsrand( QTime(0, 0, 0).secsTo(QTime::currentTime()));
            //secsTo()Ϊ���ص�ǰ������

    QApplication a(argc, argv);
    myview w;
    w.show();



    return a.exec();
}
