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



protected://派生类可访问
    void keyPressEvent(QKeyEvent *event);

    QPainterPath shape() const;//更精确的碰撞检测
    bool ison();
    bool iscrash();
    void sleep(int);
    void star_action();
    bool star_ison();

    void updatelevel();
    void updatescore();
    void updatehp();

private:


    //图案
    QImage boyright;

    QImage fly1;
    QImage fly2;
    QImage run1;
    QImage run2;



//角色

    QList<stick *> list;//管理棍子

    snake *boss;//敌人

    QGraphicsPixmapItem *star;//道具
    bool star_show;
    QVector<QImage>starflash;
    int star_flash;
    QMediaPlayer *starbell;

    QGraphicsPixmapItem *boom;//爆炸
    QMediaPlayer *bell;
    //不用qsound是因为qsound的播放与计时器有关,而QMediaPlayer的播放独立于计时器


    QMediaPlayer *music;//背景乐
    QMediaPlayer *onstict;//落在棍子上音效

    QMediaPlayer *light;

    QMediaPlaylist *playlist;



    QGraphicsSimpleTextItem *scoreitem;
    QGraphicsSimpleTextItem *levelitem;
    QGraphicsSimpleTextItem *hpitem;


//常量
    int hight;
    int width;
    int timeid;


//分数 等级 生命
    int score;

    int level;

    int hp;
     QVector<QGraphicsPixmapItem *>HPPNG;//hp的图案

    //状态参数
    int direction;
    int lastdirection;

    int state;
    bool ispause;
    int flash;//动画效果
    bool getkey;
    bool crash;//爆炸效果

    int upspeed;
    int downspeed;




};

#endif // PLAYER_H
