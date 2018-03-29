#ifndef MYVIEW_H
#define MYVIEW_H

#include"player.h"
#include"record.h"



class myview: public QGraphicsView
{
    Q_OBJECT



public:
    myview();
    void keyPressEvent(QKeyEvent *event);

public slots:
    void start_game();
    void optionshow();
    void optionhide();
    void gamecontinue();
    void gameover();
    void help();

private:
    QGraphicsScene *scene;
    record *rank;

    player* game;

    QGraphicsPixmapItem *background;
    QGraphicsPixmapItem *title;
    QGraphicsPixmapItem *over;

    //首页和游戏中需要用到的各种按钮
    QGraphicsWidget *start_button;
    QGraphicsWidget *help_button;
    QGraphicsWidget *exit_button;
    QGraphicsWidget *option_button;
    QGraphicsWidget *continue_button;
    QGraphicsWidget *restart_button;
    QGraphicsWidget *rank_button;

    //暂停时：
    QGraphicsSimpleTextItem *pause;
    QGraphicsWidget * mask;
    QGraphicsWidget * optionwidget;



    bool isover;
};

#endif // MYVIEW_H
