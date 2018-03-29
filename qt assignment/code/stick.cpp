#include "stick.h"

stick::stick()

{
    pix.load(":/png/stick.PNG");


}


QRectF stick::boundingRect() const
{
    qreal adjust =1;
    return QRectF(-pix.width()/2-adjust,-pix.height()/2-adjust,
                pix.width()+adjust*2,pix.height()+2*adjust);
}


void stick::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{

     painter->drawPixmap(boundingRect().topLeft(),pix);//»æÍ¼



}

