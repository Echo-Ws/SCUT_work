#ifndef STICK_H
#define STICK_H

#include <QGraphicsItem>
#include <QObject>
#include <QGraphicsView>
#include <QGraphicsScene>


//共用常量
enum {LEFT,RIGHT,on,off,down};

#define SCENE_W 400     //场景long
#define SCENE_H 800      //场景high

//速度类参数


class stick : public QObject, public QGraphicsItem
{
    Q_OBJECT
public:
    stick();

    QRectF boundingRect() const;   //该函数必须实现

protected:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    //重画函数

private:

    QPixmap pix;  //棍子图案




};
#endif 	// BUTTERFLY_H
