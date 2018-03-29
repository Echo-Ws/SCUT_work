#include "player.h"

#include <QKeyEvent>
#include <QMediaPlaylist>



player::player()
{
     setFlag(QGraphicsItem::ItemIsFocusable);
     setFocus();//�����н���Ż���Ӧ�����¼�

     //����ͼƬ
     boyright.load(":/png/playerright.PNG");

     fly1.load(":/png/fly1.PNG");
     fly2.load(":/png/fly2.PNG");

     run1.load(":/png/run1.PNG");
     run2.load(":/png/run2.PNG");

     //��ը
     boom=new QGraphicsPixmapItem;
     QImage temp;
     temp.load(":/png/boom.png");
     boom->setPixmap(QPixmap::fromImage(temp));

     //����
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

      //���� �ȼ�

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
      //��һ����font-family �涨Ԫ�ص�����ϵ�С�
      //font-family ���԰Ѷ������������Ϊһ�������ˡ�ϵͳ�����档����������֧�ֵ�һ�����壬��᳢����һ����Ҳ����˵��font-family ���Ե�ֵ������ĳ��Ԫ�ص����������ƻ�/���������Ƶ�һ�����ȱ��������ʹ������ʶ��ĵ�һ��ֵ��

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




      //��ʼ��stick
      for(int i = 0; i < 5; ++i)
      {
          stick *temp=new stick;
          list<<temp;

          //����ֱ��дscene()->addItem(list.at(i));
          //��Ϊ�������ʱû����scene��
      }




      //����
         music=new QMediaPlayer;

        playlist=new QMediaPlaylist;
        playlist->setPlaybackMode(QMediaPlaylist::CurrentItemInLoop);


        playlist->addMedia(QUrl::fromLocalFile(":/music/music/title.mp3"));
        //��ʼ�˵�������
        playlist->addMedia(QUrl::fromLocalFile(":/music/music/normal.mp3"));
        //��������
        playlist->addMedia(QUrl::fromLocalFile(":/music/music/over.mp3"));
        //gameover����
        //����Ե���Դ·���޷�������������֪��Ϊʲô==
         music->setPlaylist(playlist);

         playlist->setCurrentIndex(0);

          music->setVolume(50);
          music->play();




         // bell=new QSound(":/music/music/boom.wav");
         bell=new QMediaPlayer;

          bell->setMedia(QUrl::fromLocalFile(":/music/music/boom.wav"));
         //���������·����
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
     //Ƶ��Խ�� ����Ч��Խ��
    timeid=startTimer(80);



}


 void player:: init()//��ʼ��Ϸʱ�ų�ʼ��
{
     setPos(qrand()%SCENE_W+1-SCENE_W/2,-SCENE_H/2-20);


     //�ٶȲ���
     upspeed=5;
     downspeed=10;
     //��ʼ�����ֲ���

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
         //ʹstick���������һ�е�����λ��
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
    qreal adjust = 3;//���ǵ�����ʱ��ͼƬ��վ����ͼƬ��С��8
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
//��ͣ int ����
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


    }else if(state==on)//�ڹ����ϵĶ���
  {
        if(getkey)
        //��������ʱ ���߶� ������Ӧ����
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
        //���ж���
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
   QPointF a= boundingRect().bottomLeft();//ֱ���������Ļ��¶˾���̫�� �����Ķ�
   int xadjust=18;
   int yadjust=15;
   a.setX(a.x()+xadjust);
   a.setY(a.y()-yadjust);

   QPainterPath path(a);


   QPointF b= boundingRect().bottomRight();
    b.setX(b.x()-xadjust);//�ұߵĵ㴦��������
    b.setY(b.y()-yadjust);
   path.lineTo(b);


     return path;




 }





bool player::ison()
{
    for(int i = 0; i < 5; ++i)
   {
        if(  collidesWithItem (list.at(i),Qt::ContainsItemShape)  )
        {    //���this�봫��item����ײ��� �ڶ��������壺
            //The output list contains only items whose shape is fully contained inside the selection area.
            //Items that intersect with the area's outline are not included.
            //��ʱֻҪ��һֻ������������ж�����ײ

             // qDebug("player:%a    %a",this->x(),this->y());
             //qDebug("stick:%a    %a",list.at(i)->x(),list.at(i)->y());
            // �����Ӧ������ �����ӷ�Χ ���Ե���shap�Ĳ���
            return true;

        }
   }

    return false;
}


bool player::star_ison()
{
    int xadjust=50;
    int yadjust=76;
    for(int i = 0; i < 5; ++i)//��������
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
             //�����ǵ�������ʱ�ͻ���ʧ
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
        if(hp<3 && qrand()%1000<25)//���Ƶ��߳��ֵ�Ƶ�� ǰ�治�����򲻻������ȥ ��������
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





    if( ison())//�ٴμ�Ᵽ֤�ȶ���ʾ�����ڹ�����
   {
       moveBy(0,-upspeed);

   }

//////boss
    if(level>=1)
     {
        QPointF a=this->pos();
        boss->action( upspeed, downspeed, a,  list,level);

        if (crash)//���ڻ�ͼ����˳���ԭ�� sleep����������� ������ͣ������������ײǰһ�̵Ļ���
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
         //ʹ��ը������������


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




    //��������


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
              //ÿ��һ�����Ӵﵽ���� �õ�10��
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
    if(score%50==0)//��ֹ��score=0ʱ�Ĵ���
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
            //��ʱbossֻ������½��һ˲���жϷ��� ֮���ܸ�

            break;
        case 2:
            //����һֱ����ȥ ��������ײ�������bugger �ʸı�timer
            //upspeed++;
            //downspeed++;

            break;
        case 3:

            break;
        case 4:
            //boss���ܻ� �ڹ������κ�ʱ�䶼�ܱ䷽��

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
       //���ϴ�����ֱ����p����ͣ�Ϳ�ʼ

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
    if(direction!=lastdirection)//��ͼƬ�������ҵķ��洦��
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
