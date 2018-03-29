#include "myview.h"
#include<QGraphicsProxyWidget>
#include<QPushButton>
#include <QApplication>//直接关闭需要
#include<QSlider>
#include<QLabel>


#include<QMessageBox>



myview::myview()
{
     scene = new QGraphicsScene;
     scene->setSceneRect(QRectF(-SCENE_W/2,-400,SCENE_W,SCENE_H));

     rank=new record;

     game=new player;

     over=0;
     //scene->setBackgroundBrush(QPixmap(":/png/background.png"));
     //若要达到拉伸效果 需要重新drawbackground函数 还不如直接把背景图片作为一个item

     background=new QGraphicsPixmapItem;
     background->setPixmap(QPixmap(":/png/background.png"));

     background->setPos(-SCENE_W/2-50,-SCENE_H/2-90);
     scene->addItem(background);
     setBackgroundBrush(Qt::blue);

     setMinimumSize(500, 990);  //2者设置成一样说明视图尺寸不能再更改
     setMaximumSize(500, 990);

     this->setScene(scene);



     title=new QGraphicsPixmapItem;
     title->setPixmap(QPixmap(":/png/title.PNG"));

     title->setPos(-180, -350);
     scene->addItem(title);


     over=new QGraphicsPixmapItem;
     over->setPixmap(QPixmap(":/png/over.png"));

     over->setPos(-150, -350);
     scene->addItem(over);
     over->setZValue(2);
     over->hide();




//暂停：
     QWidget *temp = new QWidget;
     temp->setAutoFillBackground(true);
     temp->setPalette(QPalette(QColor(0, 0, 0, 150)));//alpha为不透明度
     temp->resize(500, 990);

     //addWidget()函数的返回值是QGraphicsProxyWidget，如果不添加相应的头文件，则此处会报错
     mask = scene->addWidget(temp);
     mask->setPos(-SCENE_W/2-50,-SCENE_H/2-100);
     mask->setZValue(1);//该层薄纱放在原图的上面，这里有点类似于opengl中的3维绘图
     mask->hide();

     pause = new QGraphicsSimpleTextItem;//第一个参数为文本内容，第二个参数为父item
     pause->setText(QString::fromLocal8Bit("游戏暂停"));
     pause->setFont(QFont("Times", 30, QFont::Light));
     pause->setPos(-80, -400);
     pause->setZValue(2);//放在第2层
     pause->setBrush(Qt::yellow);
     pause->hide();
     scene->addItem(pause);



    //设置选项
    QWidget *option = new QWidget;

    option->setPalette(QPalette(QColor(0, 0, 0, 100)));
     option->resize(300, 400);


    //将返回上级的按钮放在option上
    QPushButton *option_close_button = new QPushButton(QString::fromLocal8Bit("返回"), option);//第2个参数为按钮所在的widget
    //设置按钮的字体颜色
    QPalette palette;
    palette.setColor(QPalette::ButtonText, Qt::black);//第一个参数调色版的role，这里指的是按钮字体颜色

    QSlider *volumeSlider = new QSlider(Qt::Horizontal,option);
    volumeSlider->setRange(0, 100);
    volumeSlider->setValue(50);
    volumeSlider->move(115,105);
    connect(volumeSlider, SIGNAL(valueChanged(int)),game, SLOT(setvolume(int)));

    QLabel *volume_label = new QLabel(QString::fromLocal8Bit("音量："), option);
    volume_label->move(40,100);
    volume_label->setFont(QFont("Times", 15, QFont::Light));

    option_close_button->setPalette(palette);
    option_close_button->move(120, 200);
    option_close_button->resize(80,30);

    connect(option_close_button, SIGNAL(clicked()),this, SLOT(optionhide()));//单击后返回


    optionwidget=scene->addWidget(option);

    optionwidget->setPos(-150,-200);
    optionwidget->setZValue(2);
    optionwidget->hide();



    // 游戏中使用的按钮

    QPushButton *button1 = new QPushButton(QString::fromLocal8Bit("开    始"));
    QPushButton *button2 = new QPushButton(QString::fromLocal8Bit("选    项"));
    QPushButton *button3 = new QPushButton(QString::fromLocal8Bit("帮    助"));
    QPushButton *button4 = new QPushButton(QString::fromLocal8Bit("退    出"));
    QPushButton *button5 = new QPushButton(QString::fromLocal8Bit("继    续"));
    QPushButton *button6 = new QPushButton(QString::fromLocal8Bit("重新开始"));
    QPushButton *button7 = new QPushButton(QString::fromLocal8Bit("英 雄 榜"));

    connect(button1, SIGNAL(clicked()), this, SLOT(start_game()));

    connect(button2, SIGNAL(clicked()), this, SLOT(optionshow()));

    connect(button3, SIGNAL(clicked()), this, SLOT(help()));
    connect(button4, SIGNAL(clicked()), qApp, SLOT(quit()));
    //此处槽函数的接收对象为应用程序本身

    connect(button5, SIGNAL(clicked()), this, SLOT(gamecontinue()));
    connect(button5, SIGNAL(clicked()), game, SLOT(starttime()));

    connect(button6, SIGNAL(clicked()), game, SLOT(init()));
    connect(button6, SIGNAL(clicked()), this, SLOT(gamecontinue()));

     connect(button7, SIGNAL(clicked()), rank, SLOT(show()));

    //手指箭头标志
    button1->setCursor(QCursor(Qt::PointingHandCursor));
    button2->setCursor(QCursor(Qt::PointingHandCursor));
    button3->setCursor(QCursor(Qt::PointingHandCursor));
    button4->setCursor(QCursor(Qt::PointingHandCursor));
    button5->setCursor(QCursor(Qt::PointingHandCursor));
    button6->setCursor(QCursor(Qt::PointingHandCursor));
    button7->setCursor(QCursor(Qt::PointingHandCursor));

    start_button = scene->addWidget(button1);
    option_button = scene->addWidget(button2);
    help_button = scene->addWidget(button3);
    exit_button = scene->addWidget(button4);
    continue_button = scene->addWidget(button5);
    restart_button = scene->addWidget(button6);
    rank_button  = scene->addWidget(button7);


    start_button->setPos(-50, -100);

    option_button->setPos(-50, -20);

    help_button->setPos(-50, 60);

    exit_button->setPos(-50, 210);

    continue_button->setPos(-50, -100);

    restart_button->setPos(-50, -180);

    rank_button->setPos(-50, 140);

    start_button->resize(80,30);
    option_button->resize(80,30);
    help_button->resize(80,30);
    exit_button->resize(80,30);
    continue_button->resize(80,30);
    restart_button->resize(80,30);
    rank_button->resize(80,30);


    start_button->setZValue(2);
    option_button->setZValue(2);
    help_button->setZValue(2);
    exit_button->setZValue(2);
    continue_button->setZValue(2);
    restart_button->setZValue(2);
    rank_button->setZValue(2);

    continue_button->hide();
    restart_button->hide();

//游戏结束

    connect(game, SIGNAL(gameover(int)), rank, SLOT(can_save(int)));

    connect(game, SIGNAL(gameover(int)), this, SLOT(gameover()));

}



void myview::optionshow()
{
    optionwidget->show();
    if(mask->isVisible())
                //通过此来判断是否处于暂停
   {
        continue_button->hide();
        restart_button->hide();
   }else
         start_button->hide();

    option_button->hide();
    help_button->hide();
    exit_button->hide();
    rank_button->hide();

}

void myview::optionhide()
{
    optionwidget->hide();

    if(!mask->isVisible())
        start_button->show();
    else
    {
        continue_button->show();
        restart_button->show();
    }
    option_button->show();
    help_button->show();
    exit_button->show();
    rank_button->show();


}


void myview::help()
{
    QMessageBox  a(QMessageBox::Information,
                   QString::fromLocal8Bit("操作说明"),
                   QString::fromLocal8Bit("用左右键控制人物移动。P键暂停。hp减少时，请及时吃星星补充hp。hp为0时游戏结束。掉到最低端直接死亡,在顶端会受到雷击减hp.注意：随着等级变化，速度也会变。"));

    a.exec();



}


void myview::gamecontinue()
{
    continue_button->hide();
    option_button->hide();
    help_button->hide();
    exit_button->hide();
    restart_button->hide();
    rank_button->hide();
    restart_button->setPos(-50, -180);
    //重新开始时把over改动的位置还原

    mask->hide();
    pause->hide();
    over->hide();

}


void myview::start_game()
{
    //游戏初始化


    for(int i = 0; i < 5; ++i)
    {
        scene->addItem(game->return_stick(i));//要在boy之前加入scene，或者setzvalue
    }

    scene->addItem(game);
    game->init();

    title->hide();

    start_button->hide();
    option_button->hide();
    help_button->hide();
    exit_button->hide();
    rank_button->hide();


}

void myview::gameover()
{

    mask->show();
    restart_button->show();
    exit_button->show();
    over->show();
    rank_button->show();
    help_button->show();
    option_button->show();

    option_button->setPos(-50, -20);

    help_button->setPos(-50, 60);

    exit_button->setPos(-50, 210);



    restart_button->setPos(-50, -100);




}



void myview:: keyPressEvent(QKeyEvent *event)
{
    game->setFocus();
    switch(event->key())
    {
    case Qt::Key_P:

        if(!over->isVisible())//防止游戏结束后又输入p
        {
            mask->show();
            pause->show();
            option_button->show();
            help_button->show();
            exit_button->show();
            continue_button->show();
            restart_button->show();
            rank_button->show();
        }

        break;



    }

    QGraphicsView::keyPressEvent(event);
    //让键盘事件可传给item

}
