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

    //��ҳ����Ϸ����Ҫ�õ��ĸ��ְ�ť
    QGraphicsWidget *start_button;
    QGraphicsWidget *help_button;
    QGraphicsWidget *exit_button;
    QGraphicsWidget *option_button;
    QGraphicsWidget *continue_button;
    QGraphicsWidget *restart_button;
    QGraphicsWidget *rank_button;

    //��ͣʱ��
    QGraphicsSimpleTextItem *pause;
    QGraphicsWidget * mask;
    QGraphicsWidget * optionwidget;



    bool isover;
};

#endif // MYVIEW_H
