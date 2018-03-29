#include "player.h"

#include <QKeyEvent>
#include <QMediaPlaylist>



player::player()
{
     setFlag(QGraphicsItem::ItemIsFocusable);
     setFocus();//必须有焦点才会响应键盘事件

     //载入图片
     boyright.load(":/png/playerright.PNG");

     fly1.load(":/png/fly1.PNG");
     fly2.load(":/png/fly2.PNG");

     run1.load(":/png/run1.PNG");
     run2.load(":/png/run2.PNG");

     //爆炸
     boom=new QGraphicsPixmapItem;
     QImage temp;
     temp.load(":/png/boom.png");
     boom->setPixmap(QPixmap::fromImage(temp));

     //星星
     star=new QGraphicsPixmapItem;

     temp.load(":/png/star1.PNG");
     starflash<<temp;
      temp.load(":/png/star2.PNG");
       starflash<<temp;
      temp.load(":/png/star3.PNG");
       starflash<<temp;
      temp.load(":/png/star4.PNG");
       starflash<<temp;
      temp.load(":/png/star5.PNG");
      starflash<<temp;
      temp.load(":/png/star4.PNG");
      temp.mirrored(1,0);
      starflash<<temp;
      temp.load(":/png/star3.PNG");
      temp.mirrored(1,0);
      starflash<<temp;
      temp.load(":/png/star2.PNG");
      temp.mirrored(1,0);
      starflash<<temp;


      star_show=0;




      hight=boyright.size().height();
      width=boyright.size().width();

      direction=RIGHT;
      lastdirection=direction;


      //boss
      boss=new snake;

      crash=0;

      //分数 等级

      scoreitem=new QGraphicsSimpleTextItem;
      levelitem=new QGraphicsSimpleTextItem;
      hpitem=new QGraphicsSimpleTextItem;

      scoreitem->setPos(100,-SCENE_H/2-95);
      levelitem->setPos(0,-SCENE_H/2-95);
      hpitem->setPos(-250,-SCENE_H/2-95);

      scoreitem->setBrush(Qt::yellow);
      levelitem->setBrush(Qt::yellow);
      hpitem->setBrush(Qt::yellow);

      levelitem->setFont(QFont("times",15));
      scoreitem->setFont(QFont("times",15));
      hpitem->setFont(QFont("times",15));
      //第一参数font-family 规定元素的字体系列。
      //font-family 可以把多个字体名称作为一个“回退”系统来保存。如果浏览器不支持第一个字体，则会尝试下一个。也就是说，font-family 属性的值是用于某个元素的字体族名称或/及类族名称的一个优先表。浏览器会使用它可识别的第一个值。

      scoreitem->setZValue(2);
      levelitem->setZValue(2);
      hpitem->setZValue(2);



      temp.load(":/png/hp.PNG");
      hp=3;


      for(int i=0;i<hp;i++)
      {
          QGraphicsPixmapItem *t=new QGraphicsPixmapItem;
          t->setPixmap(QPixmap::fromImage(temp));
          HPPNG<<t;
          HPPNG.at(i)->setPos(-210+i*30,-SCENE_H/2-90);

          HPPNG.at(i)->setZValue(2);
      }




      //初始化stick
      for(int i = 0; i < 5; ++i)
      {
          stick *temp=new stick;
          list<<temp;

          //不能直接写scene()->addItem(list.at(i));
          //因为构造出来时没有在scene中
      }




      //音乐
         music=new QMediaPlayer;

        playlist=new QMediaPlaylist;
        playlist->setPlaybackMode(QMediaPlaylist::CurrentItemInLoop);


        playlist->addMedia(QUrl::fromLocalFile(":/music/music/title.mp3"));
        //开始菜单的音乐
        playlist->addMedia(QUrl::fromLocalFile(":/music/music/normal.mp3"));
        //背景音乐
        playlist->addMedia(QUrl::fromLocalFile(":/music/music/over.mp3"));
        //gameover音乐
        //用相对的资源路径无法播出声音，不知道为什么==
         music->setPlaylist(playlist);

         playlist->setCurrentIndex(0);

          music->setVolume(50);
          music->play();




         // bell=new QSound(":/music/music/boom.wav");
         bell=new QMediaPlayer;

          bell->setMedia(QUrl::fromLocalFile(":/music/music/boom.wav"));
         //不能用相对路径，
           bell->setVolume(50);

          starbell=new QMediaPlayer;
          starbell->setMedia(QUrl::fromLocalFile(":/music/music/get.wav"));
          starbell->setVolume(50);


          onstict=new QMediaPlayer;
          onstict->setMedia(QUrl::fromLocalFile(":/music/music/on.mp3"));
          onstict->setVolume(50);

          light=new QMediaPlayer;
          light->setMedia(QUrl::fromLocalFile(":/music/music/light.mp3"));
          light->setVolume(50);
          qDebug("!!! %d",music->volume());

}


void player::setvolume(int volume)
{
    music->setVolume(volume);


    music->play();
    bell->setVolume(volume);
    starbell->setVolume(volume);
    onstict->setVolume(volume);
    light->setVolume(volume);

    qDebug("!!! %d",music->volume());

}


void player::starttime()
{
     //频率越高 动画效果越好
    timeid=startTimer(80);



}


 void player:: init()//开始游戏时才初始化
{
     setPos(qrand()%SCENE_W+1-SCENE_W/2,-SCENE_H/2-20);


     //速度参数
     upspeed=5;
     downspeed=10;
     //初始化各种参数

     getkey=0;
     ispause=0;
     state=off;

     score=0;
     level=0;
     hp=3;

     for(int i=0;i<hp;i++)
     {

         HPPNG.at(i)->show();

         scene()->addItem( HPPNG.at(i));
     }


     flash=0;
     star_flash=0;

     if(star_show)
     {
         scene()->removeItem(star);
         star_show=0;
     }

     playlist->setCurrentIndex(1);
     playlist->setPlaybackMode(QMediaPlaylist::CurrentItemInLoop);
     music->play();

     scoreitem->setText(QString("score: %1").arg(score));
     levelitem->setText(QString("Level: %1").arg(level));
     hpitem->setText(QString("HP: "));



     scene()->addItem(scoreitem);
     scene()->addItem(levelitem);
     scene()->addItem(hpitem);



     for(int i = 0; i < 5; ++i)
     {
         list.at(i)->setPos(qrand()%SCENE_W+1-SCENE_W/2,SCENE_H/2+20-160*i);
         //使stick随机出现在一行的任意位置
     }


      if(level>=1)
      {
          boss->hide();
         scene()->removeItem(boss);
      }
      boss->setPos(qrand()%SCENE_W+1-SCENE_W/2,-SCENE_H/2-150);

     starttime();








}


QRectF player::boundingRect() const
{
    qreal adjust = 3;//考虑到飞行时的图片和站立的图片大小差8
    return QRectF(-width/2-adjust,-hight/2-adjust,
                width+adjust*2,hight+2*adjust);
}




bool  player::iscrash()
 {
     if(collidesWithItem(boss,Qt::IntersectsItemBoundingRect))
     {

              return true;
     }

             return false;
 }


void player::sleep(int msec)
//暂停 int 毫秒
{
    QTime dieTime = QTime::currentTime().addMSecs(msec);

    killTimer(timeid);

    while( QTime::currentTime()<dieTime);

    timeid=startTimer(80-level*2);
}



void player::paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
                   QWidget *widget)
{
    painter->setPen(Qt::NoPen);
    if(this->y()<-SCENE_H/2-20)
    {

        QImage t1;
        t1.load(":/png/light.PNG");
         QImage t2;
         t2.load(":/png/light2.PNG");
        flash?
         painter->drawPixmap(boundingRect().topLeft(),
                          QPixmap::fromImage(t1)):
         painter->drawPixmap(boundingRect().topLeft(),
                          QPixmap::fromImage(t2));
        flash=!flash;



        light->play();

        if(this->y()<-SCENE_H/2-40)
        {
            hp--;
            updatehp();
            this->moveBy(0,80);
        }


    }else if(state==on)//在棍子上的动作
  {
        if(getkey)
        //当有输入时 即走动 播放相应动画
        {
            flash?
             painter->drawPixmap(boundingRect().topLeft(),
                              QPixmap::fromImage(run1)):
             painter->drawPixmap(boundingRect().topLeft(),
                              QPixmap::fromImage(run2));
            flash=!flash;
        }else{

          painter->drawPixmap(boundingRect().topLeft(),
                        QPixmap::fromImage(boyright));

        }

  }
    else if(state==off)
        //飞行动作
    {

      flash?
      painter->drawPixmap(boundingRect().topLeft(),
                        QPixmap::fromImage(fly1)):
      painter->drawPixmap(boundingRect().topLeft(),
                        QPixmap::fromImage(fly2));
      flash=!flash;


    }


}




QPainterPath player:: shape() const
 {
   QPointF a= boundingRect().bottomLeft();//直接用这个点的话下端距离太长 需做改动
   int xadjust=18;
   int yadjust=15;
   a.setX(a.x()+xadjust);
   a.setY(a.y()-yadjust);

   QPainterPath path(a);


   QPointF b= boundingRect().bottomRight();
    b.setX(b.x()-xadjust);//右边的点处于正半轴
    b.setY(b.y()-yadjust);
   path.lineTo(b);


     return path;




 }





bool player::ison()
{
    for(int i = 0; i < 5; ++i)
   {
        if(  collidesWithItem (list.at(i),Qt::ContainsItemShape)  )
        {    //检测this与传入item的碰撞情况 第二参数意义：
            //The output list contains only items whose shape is fully contained inside the selection area.
            //Items that intersect with the area's outline are not included.
            //此时只要有一只脚在外面则会判断无碰撞

             // qDebug("player:%a    %a",this->x(),this->y());
             //qDebug("stick:%a    %a",list.at(i)->x(),list.at(i)->y());
            // 输出相应的坐标 检测棍子范围 可以调整shap的参数
            return true;

        }
   }

    return false;
}


bool player::star_ison()
{
    int xadjust=50;
    int yadjust=76;
    for(int i = 0; i < 5; ++i)//操作棍子
   {
        if( star->x()>=list.at(i)->x()-xadjust-30&&star->x()<=list.at(i)->x()+xadjust-20
                &&star->y()<=list.at(i)->y()-yadjust+8&&star->y()>=list.at(i)->y()-yadjust-5 )
          {
            qDebug("STICK x  %a    y   %a    ",list.at(i)->x(),list.at(i)->y());
            star->setY(list.at(i)->y()-yadjust);
            return true;
         }
   }
    return false;
}


void player::star_action()
{
    star->setPixmap(QPixmap::fromImage(starflash.at(star_flash)));
    star_flash < starflash.count()-1?
            star_flash++:
           star_flash=0;




    if(star_ison())
    {
         star->moveBy(0,-8);


    }else{
         star->moveBy(0,downspeed);

         if(star->y()>SCENE_H/2+20)
             //当星星掉到底下时就会消失
         {
             scene()->removeItem(star);
             star_show=0;
         }
    }


    if(collidesWithItem(star,Qt::IntersectsItemBoundingRect))
    {
        scene()->removeItem(star);
        star_show=0;

        starbell->play();
        updatescore();
        qDebug(" star score  %d    ",score);
        updatelevel();

        hp++;
         updatehp();


    }


}




 void player::timerEvent(QTimerEvent *)
{
    getkey=0;

//////star:

    if(star_show)
    {
        star_action();
    }else{
        if(hp<3 && qrand()%1000<25)//控制道具出现的频率 前面不成立则不会进行下去 减少运算
        {
            star->setPos(qrand()%SCENE_W+1-SCENE_W/2,-SCENE_H/2-40);
            scene()->addItem(star);
            star_show=1;
        }
    }




////// player:

     if( ison())
    {
         moveBy(0,-upspeed);

         if(state==off)
         {
             onstict->play();
         }
         state=on;
    }
   else
     {
         moveBy(0,downspeed);
         state=off;


     }

     if(pos().y()>SCENE_H/2+20)
    {
        hp=0;
        updatehp();



    }





    if( ison())//再次检测保证稳定显示人物在棍子上
   {
       moveBy(0,-upspeed);

   }

//////boss
    if(level>=1)
     {
        QPointF a=this->pos();
        boss->action( upspeed, downspeed, a,  list,level);

        if (crash)//由于绘图更新顺序的原因 sleep必须放在这里 否则暂停画面总是在碰撞前一刻的画面
         {


            sleep(500);
             qDebug("stoptime:%d    ",QTime::currentTime().msec());
             boss->setPos(qrand()%SCENE_W+1-SCENE_W/2,-SCENE_H/2-20);
             boss->show();
             this->show();
             this->setFocus();
             scene()->removeItem(boom);
             hp--;
             updatehp();


         }

     if(iscrash())
     {

         boom->setPos(this->pos());

         boom->moveBy(-width,-hight/2);
         //使爆炸中心在人物上


         scene()->addItem(boom);
         bell->play();
         boss->hide();
         this->hide();

         crash=1;

         qDebug("itemtime:%d    ",QTime::currentTime().msec());








     }else{
         crash=0;
     }
    }




    //操作棍子


        for(int i = 0; i < 5; ++i)
      {
         list.at(i)->moveBy(0,-upspeed);
         if( list.at(i)->pos().y()<-SCENE_H/2-20)
         {
              list.at(i)->setY(SCENE_H/2+20);
              list.at(i)->setX(qrand()%SCENE_W+1-SCENE_W/2);

              updatescore();
              updatelevel();
              qDebug("score  %d    ",score);
              //每有一根棍子达到顶点 得到10分
         }



      }

    update();


}



  void player::updatescore()
 {
     score+=10;

     scoreitem->setText(QString("score: %1").arg(score));




 }



 void player::updatelevel()
{
    if(score%50==0)//防止在score=0时的错误
    {
        level++;
        levelitem->setText(QString("Level: %1").arg(level));
        qDebug("level  %d    ",level);
        killTimer(timeid);
        timeid=startTimer(80-2*level);
        switch (level)
        {
        case 1:
            scene()->addItem(boss);
            //此时boss只能在着陆的一瞬间判断方向 之后不能改

            break;
        case 2:
            //不能一直加下去 否则在碰撞检测会出现bugger 故改变timer
            //upspeed++;
            //downspeed++;

            break;
        case 3:

            break;
        case 4:
            //boss智能化 在棍子上任何时间都能变方向

             break;


    }



    }
}


 void player::updatehp()
 {
         int t=0;

         for(t;t<hp;t++)
         {
             HPPNG.at(t)->show();
         }


         for(t=hp;t<3;t++)
         {
             HPPNG.at(t)->hide();
         }

         if(hp==0)
         {
             playlist->setCurrentIndex(2);
             playlist->setPlaybackMode(QMediaPlaylist::CurrentItemOnce);
             music->play();
             killTimer(timeid);
             emit gameover(score);


         }



 }

 void  player::keyPressEvent(QKeyEvent *event)
{


    switch(event->key())
    {
    case Qt::Key_P:
       // ispause?
       // timeid=startTimer(100):killTimer(timeid);
       // ispause=!ispause;
       //以上代码是直接用p来暂停和开始

        killTimer(timeid);
       // qDebug("star x  %a    y   %a    ",star->x(),star->y());
        break;

    case Qt::Key_Left:
        if(this->x()>-223)
        {
            moveBy(-10, 0);
        }
        direction=LEFT;
         getkey=1;
         break;

    case Qt::Key_Right:
         if(this->x()<223)
             moveBy(10, 0);
        direction=RIGHT;
        getkey=1;
        break;

    case Qt::Key_A:
        star->moveBy(-10,0);
        qDebug("play x  %a    y   %a    ",this->x(),this->y());
        break;

    case Qt::Key_D:
        star->moveBy(10,0);
        break;

    case Qt::Key_W:

       updatescore();
        updatelevel();
        break;

    }
    if(direction!=lastdirection)//对图片进行左右的翻面处理
    {
        boyright=boyright.mirrored(1,0);
        run1=run1.mirrored(1,0);
        run2=run2.mirrored(1,0);
        fly1=fly1.mirrored(1,0);
        fly2=fly2.mirrored(1,0);
    }

    lastdirection=direction;
    update();
}


 stick* player::return_stick(int i)
{
 return  list.at(i);
}
