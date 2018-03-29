#ifndef PLAYER_H
#define PLAYER_H

#include <QGraphicsItem>
#include<QPainter>
#include<QTime>
#include<QtMultimedia/QMediaPlayer>
#include<QSound>
#include"snake.h"
#include <QMediaPlayer>
#include <QKeyEvent>


class player:public QObject,public QGraphicsItem
{

    Q_OBJECT
public:
    player();
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
                  QWidget *widget);
    void timerEvent(QTimerEvent *);
    stick* return_stick(int);
    int rscore()
    {
        return score;
    }



public slots:
    void init();
    void setvolume(int volume);
    void starttime();

signals:
    void gameover(int);



protected://������ɷ���
    void keyPressEvent(QKeyEvent *event);

    QPainterPath shape() const;//����ȷ����ײ���
    bool ison();
    bool iscrash();
    void sleep(int);
    void star_action();
    bool star_ison();

    void updatelevel();
    void updatescore();
    void updatehp();

private:


    //ͼ��
    QImage boyright;

    QImage fly1;
    QImage fly2;
    QImage run1;
    QImage run2;



//��ɫ

    QList<stick *> list;//�������

    snake *boss;//����

    QGraphicsPixmapItem *star;//����
    bool star_show;
    QVector<QImage>starflash;
    int star_flash;
    QMediaPlayer *starbell;

    QGraphicsPixmapItem *boom;//��ը
    QMediaPlayer *bell;
    //����qsound����Ϊqsound�Ĳ������ʱ���й�,��QMediaPlayer�Ĳ��Ŷ����ڼ�ʱ��


    QMediaPlayer *music;//������
    QMediaPlayer *onstict;//���ڹ�������Ч

    QMediaPlayer *light;

    QMediaPlaylist *playlist;



    QGraphicsSimpleTextItem *scoreitem;
    QGraphicsSimpleTextItem *levelitem;
    QGraphicsSimpleTextItem *hpitem;


//����
    int hight;
    int width;
    int timeid;


//���� �ȼ� ����
    int score;

    int level;

    int hp;
     QVector<QGraphicsPixmapItem *>HPPNG;//hp��ͼ��

    //״̬����
    int direction;
    int lastdirection;

    int state;
    bool ispause;
    int flash;//����Ч��
    bool getkey;
    bool crash;//��ըЧ��

    int upspeed;
    int downspeed;




};

#endif // PLAYER_H
