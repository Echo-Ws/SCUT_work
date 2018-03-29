#include "snake.h"
#include<QMatrix>

snake::snake()
{
    head1.load(":/png/fire.PNG");
    head2.load(":/png/fire2.PNG");

    QMatrix matrix;
    matrix.rotate(-90);
    head1=head1.transformed(matrix);
    head2=head2.transformed(matrix);


    width=head2.width();
    height=head2.height();
    flash=0;
    grow=2;
    horizontal_speed=5;
    direction=down;
    lastdirection=direction;



}



QRectF snake::boundingRect() const
{
    qreal adjust =1;
    return QRectF(-width/2-adjust,-height/2-adjust,
             head2.width()+adjust*2,head2.height()+2*adjust);



}



QPainterPath snake:: shape() const
{
    QPointF a= boundingRect().bottomLeft();//直接用这个点的话下端距离太长 需做改动
    int xadjust=30;
    int yadjust=10;
    a.setX(a.x()+xadjust);
    a.setY(a.y()-yadjust);

    QPainterPath path(a);


    QPointF b= boundingRect().bottomRight();
     b.setX(b.x()-xadjust);//右边的点处于正半轴
     b.setY(b.y()-yadjust);
    path.lineTo(b);


      return path;
}





bool snake::isColliding(const QList<stick *>  list)
{
    int xadjust=60;
    int yadjust=30;
    for(int i = 0; i < 5; ++i)//操作棍子
   {
        if( this->x()>=list.at(i)->x()-xadjust&&this->x()<=list.at(i)->x()+xadjust
                &&this->y()<=list.at(i)->y()-yadjust+10&&this->y()>=list.at(i)->y()-yadjust-10 )
          {
            //qDebug("boss:%a    %a",this->x(),this->y());
            //qDebug("bosssstick:%a    %a",list.at(i)->x(),list.at(i)->y());
            //调整具体参数
            this->setY(list.at(i)->y()-yadjust);
            return true;
         }
   }
    return false;
}


void snake::paint(QPainter *painter,
           const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->setPen(Qt::NoPen);

    flash?
    painter->drawPixmap(boundingRect().topLeft(),
                      QPixmap::fromImage(head1)):
    painter->drawPixmap(boundingRect().topLeft(),
                      QPixmap::fromImage(head2));

    flash=!flash;







}



void snake::action(int upspeed,int downspeed,QPointF player, QList<stick *>  list,int level)
{

    QPointF head=this->pos();


    if(isColliding(list))
    {
        moveBy(0,-upspeed);
       if(lastdirection==down)//落在棍子的一刻
       {

           if(head.x()>player.x())
           {

               direction=LEFT;
               QMatrix matrix;
               matrix.rotate(90);
               head1=head1.transformed(matrix);
               head2=head2.transformed(matrix);
               moveBy(horizontal_speed,0);
           }else{
               direction=RIGHT;
               QMatrix matrix;
               matrix.rotate(90);
               head1=head1.transformed(matrix);
               head2=head2.transformed(matrix);
               head1=head1.mirrored(1,0);
               head2=head2.mirrored(1,0);
               moveBy(-horizontal_speed,0);
           }

       }

       //跟踪效果 lv4解锁
       if(level>=4)
       {
           if(head.x()>player.x())

       {
           if(lastdirection==RIGHT)
             {
               head1=head1.mirrored(1,0);
               head2=head2.mirrored(1,0);
              }
               direction=LEFT;


       }
           else {

           if(lastdirection==LEFT)
             {
               head1=head1.mirrored(1,0);
               head2=head2.mirrored(1,0);
              }

           direction=RIGHT;

       }
       }

       direction==RIGHT?
       moveBy(horizontal_speed,0):
       moveBy(-horizontal_speed,0);

    }else{
        direction=down;
        if(lastdirection==RIGHT)
        {
            QMatrix matrix;
            matrix.rotate(-90);
            head1= head1.mirrored(1,0);
            head2= head2.mirrored(1,0);
            head1= head1.transformed(matrix);
            head2= head2.transformed(matrix);

        }

        else if(lastdirection==LEFT)
        {
            QMatrix matrix;
            matrix.rotate(-90);
            head1=head1.transformed(matrix);
            head2=head2.transformed(matrix);
        }



       moveBy(0,downspeed);

    }


    if( isColliding(list))
   {
       moveBy(0,-upspeed);

   }
    if(y()>SCENE_H/2+20)
    {
        setY(-SCENE_H/2-20);
        setX(qrand()%SCENE_W+1-SCENE_W/2);
    }


    lastdirection=direction;

}




