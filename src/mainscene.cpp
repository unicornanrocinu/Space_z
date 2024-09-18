#include "mainscene.h"
#include "ui_mainscene.h"
#include <QPainter>
#include "mypushbutton.h"
#include <QScreen>
#include <QDebug>
#include <QTimer>

MainScene::MainScene(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainScene)
{
    ui->setupUi(this);

    //设置主界面
    setWindowIcon(QIcon(":/res/icon.png"));
    setWindowTitle(QString("异形工场"));
    setFixedSize(1500,800);


    //开始按钮
    MyPushButton *start_btn=new MyPushButton(this,":/res/start_button.png");
    start_btn->setParent(this);
    start_btn->move(this->width()*0.5-start_btn->width()*0.5,this->height()*0.5);
    connect(start_btn,MyPushButton::clicked,[&](){
        map_p=new mapscene(this);
        map_p->setWindowFlags(Qt::Window);
        //退出时父界面弹出
        connect(map_p,mapscene::windowClosed,[=](){
            this->show();
        });
        //延时进入
        QTimer::singleShot(500,this,[=]{
            this->hide();
            map_p->show();
        });
    });

    //存档按钮
    MyPushButton *store_btn=new MyPushButton(this,":/res/store_button.png");
    store_btn->setParent(this);
    store_btn->move(this->width()*0.5-store_btn->width()*0.5,this->height()*0.5+100);
    connect(store_btn,MyPushButton::clicked,[&](){
    //TODO
    });



    //商店按鈕
    MyPushButton *shop_btn=new MyPushButton(this,":/res/shop_button.png");
    shop_btn->setParent(this);
    shop_btn->move(this->width()*0.5-shop_btn->width()*0.5,this->height()*0.5+200);
    connect(shop_btn,MyPushButton::clicked,[&](){
        //TODO
    });

    //退出按钮
    MyPushButton *exit_btn=new MyPushButton(this,":/res/exit_button.png","",0.5);
    exit_btn->setParent(this);
    exit_btn->move(this->width()-exit_btn->width(),0);
    connect(exit_btn,MyPushButton::clicked,[&](){
        this->close();
    });


    money=9999;
}

void MainScene::paintEvent(QPaintEvent *event){
    QPainter painter(this);
    QPixmap pix;
    pix.load(":/res/background.png");
    painter.drawPixmap(0,0,this->width(),this->height(),pix);
    pix.load(":/res/title_image.png");
    painter.drawPixmap(this->width()*0.5-pix.width()*0.5,this->height()*0.5-pix.width()*0.5,pix);
    pix.load(":/res/money.png");
    painter.drawPixmap(0,0,0.06*this->width(),0.12*this->height(),pix);
    QFont font("Arial",30); // 字体为Arial，大小为16点
    painter.setFont(font);
    painter.drawText(0.06*this->width(),0.06*this->height()-25,250,100,Qt::AlignLeft,QString::number(money));
}

MainScene::~MainScene()
{
    delete ui;
}

