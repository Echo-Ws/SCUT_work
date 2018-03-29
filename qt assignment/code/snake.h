#ifndef SNAKE_H

#include"stick.h"
class snake:public QGraphicsItem,public QObject
{
public:
    snake();
    QRectF boundingRect() const;
    void paint(QPainter *painter,
               const QStyleOptionGraphicsItem *option, QWidget *widget);

   void action(int,int,QPointF,QList<stick *>,int);
   //�����ٶ� �½��ٶ� playerλ��,level
    QPainterPath shape() const;

    int rdirection(){return direction;}

    int rlastdirection(){return lastdirection;}

    void setlastdirection(int a){lastdirection=a;}

    void setdirection(int a){direction=a;}


private:
   int direction;
   int lastdirection;
   int flash;
   int grow;
   int horizontal_speed;//ˮƽ�ٶ�
   QImage head1;
   QImage head2;

   int height;
   int width;



   bool isColliding(const QList<stick *>  list);



};

#endif // SNAKE_H
