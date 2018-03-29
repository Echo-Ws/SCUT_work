#ifndef STICK_H
#define STICK_H

#include <QGraphicsItem>
#include <QObject>
#include <QGraphicsView>
#include <QGraphicsScene>


//���ó���
enum {LEFT,RIGHT,on,off,down};

#define SCENE_W 400     //����long
#define SCENE_H 800      //����high

//�ٶ������


class stick : public QObject, public QGraphicsItem
{
    Q_OBJECT
public:
    stick();

    QRectF boundingRect() const;   //�ú�������ʵ��

protected:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    //�ػ�����

private:

    QPixmap pix;  //����ͼ��




};
#endif 	// BUTTERFLY_H
