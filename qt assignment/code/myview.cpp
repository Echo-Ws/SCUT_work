#include "myview.h"
#include<QGraphicsProxyWidget>
#include<QPushButton>
#include <QApplication>//ֱ�ӹر���Ҫ
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
     //��Ҫ�ﵽ����Ч�� ��Ҫ����drawbackground���� ������ֱ�Ӱѱ���ͼƬ��Ϊһ��item

     background=new QGraphicsPixmapItem;
     background->setPixmap(QPixmap(":/png/background.png"));

     background->setPos(-SCENE_W/2-50,-SCENE_H/2-90);
     scene->addItem(background);
     setBackgroundBrush(Qt::blue);

     setMinimumSize(500, 990);  //2�����ó�һ��˵����ͼ�ߴ粻���ٸ���
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




//��ͣ��
     QWidget *temp = new QWidget;
     temp->setAutoFillBackground(true);
     temp->setPalette(QPalette(QColor(0, 0, 0, 150)));//alphaΪ��͸����
     temp->resize(500, 990);

     //addWidget()�����ķ���ֵ��QGraphicsProxyWidget������������Ӧ��ͷ�ļ�����˴��ᱨ��
     mask = scene->addWidget(temp);
     mask->setPos(-SCENE_W/2-50,-SCENE_H/2-100);
     mask->setZValue(1);//�ò㱡ɴ����ԭͼ�����棬�����е�������opengl�е�3ά��ͼ
     mask->hide();

     pause = new QGraphicsSimpleTextItem;//��һ������Ϊ�ı����ݣ��ڶ�������Ϊ��item
     pause->setText(QString::fromLocal8Bit("��Ϸ��ͣ"));
     pause->setFont(QFont("Times", 30, QFont::Light));
     pause->setPos(-80, -400);
     pause->setZValue(2);//���ڵ�2��
     pause->setBrush(Qt::yellow);
     pause->hide();
     scene->addItem(pause);



    //����ѡ��
    QWidget *option = new QWidget;

    option->setPalette(QPalette(QColor(0, 0, 0, 100)));
     option->resize(300, 400);


    //�������ϼ��İ�ť����option��
    QPushButton *option_close_button = new QPushButton(QString::fromLocal8Bit("����"), option);//��2������Ϊ��ť���ڵ�widget
    //���ð�ť��������ɫ
    QPalette palette;
    palette.setColor(QPalette::ButtonText, Qt::black);//��һ��������ɫ���role������ָ���ǰ�ť������ɫ

    QSlider *volumeSlider = new QSlider(Qt::Horizontal,option);
    volumeSlider->setRange(0, 100);
    volumeSlider->setValue(50);
    volumeSlider->move(115,105);
    connect(volumeSlider, SIGNAL(valueChanged(int)),game, SLOT(setvolume(int)));

    QLabel *volume_label = new QLabel(QString::fromLocal8Bit("������"), option);
    volume_label->move(40,100);
    volume_label->setFont(QFont("Times", 15, QFont::Light));

    option_close_button->setPalette(palette);
    option_close_button->move(120, 200);
    option_close_button->resize(80,30);

    connect(option_close_button, SIGNAL(clicked()),this, SLOT(optionhide()));//�����󷵻�


    optionwidget=scene->addWidget(option);

    optionwidget->setPos(-150,-200);
    optionwidget->setZValue(2);
    optionwidget->hide();



    // ��Ϸ��ʹ�õİ�ť

    QPushButton *button1 = new QPushButton(QString::fromLocal8Bit("��    ʼ"));
    QPushButton *button2 = new QPushButton(QString::fromLocal8Bit("ѡ    ��"));
    QPushButton *button3 = new QPushButton(QString::fromLocal8Bit("��    ��"));
    QPushButton *button4 = new QPushButton(QString::fromLocal8Bit("��    ��"));
    QPushButton *button5 = new QPushButton(QString::fromLocal8Bit("��    ��"));
    QPushButton *button6 = new QPushButton(QString::fromLocal8Bit("���¿�ʼ"));
    QPushButton *button7 = new QPushButton(QString::fromLocal8Bit("Ӣ �� ��"));

    connect(button1, SIGNAL(clicked()), this, SLOT(start_game()));

    connect(button2, SIGNAL(clicked()), this, SLOT(optionshow()));

    connect(button3, SIGNAL(clicked()), this, SLOT(help()));
    connect(button4, SIGNAL(clicked()), qApp, SLOT(quit()));
    //�˴��ۺ����Ľ��ն���ΪӦ�ó�����

    connect(button5, SIGNAL(clicked()), this, SLOT(gamecontinue()));
    connect(button5, SIGNAL(clicked()), game, SLOT(starttime()));

    connect(button6, SIGNAL(clicked()), game, SLOT(init()));
    connect(button6, SIGNAL(clicked()), this, SLOT(gamecontinue()));

     connect(button7, SIGNAL(clicked()), rank, SLOT(show()));

    //��ָ��ͷ��־
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

//��Ϸ����

    connect(game, SIGNAL(gameover(int)), rank, SLOT(can_save(int)));

    connect(game, SIGNAL(gameover(int)), this, SLOT(gameover()));

}



void myview::optionshow()
{
    optionwidget->show();
    if(mask->isVisible())
                //ͨ�������ж��Ƿ�����ͣ
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
                   QString::fromLocal8Bit("����˵��"),
                   QString::fromLocal8Bit("�����Ҽ����������ƶ���P����ͣ��hp����ʱ���뼰ʱ�����ǲ���hp��hpΪ0ʱ��Ϸ������������Ͷ�ֱ������,�ڶ��˻��ܵ��׻���hp.ע�⣺���ŵȼ��仯���ٶ�Ҳ��䡣"));

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
    //���¿�ʼʱ��over�Ķ���λ�û�ԭ

    mask->hide();
    pause->hide();
    over->hide();

}


void myview::start_game()
{
    //��Ϸ��ʼ��


    for(int i = 0; i < 5; ++i)
    {
        scene->addItem(game->return_stick(i));//Ҫ��boy֮ǰ����scene������setzvalue
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

        if(!over->isVisible())//��ֹ��Ϸ������������p
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
    //�ü����¼��ɴ���item

}
