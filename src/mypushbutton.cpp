#include "mypushbutton.h"
#include <QDebug>


MyPushButton::MyPushButton(QWidget *parent,QString normalImg,QString pressImg,double scale):QPushButton(parent),scale(scale),is_chosen(0)
{
    this->normalImgPath=normalImg;
    this->PressImgPath=pressImg;

    QPixmap pix;
    bool ret =pix.load(normalImgPath);
    if(!ret){
        qDebug() << "加载失败";
        return;
    }//加载图片

    this->resize(pix.width()*scale,pix.height()*scale);

    this->setStyleSheet("QPushButton {border:0px;}");//设置不规则样式

    this->setIcon(pix);

    this->setIconSize((QSize(pix.width()*scale,pix.height()*scale)));
}



//弃案，点击按键变化

// void MyPushButton::chosen(){
//     if(is_chosen)
//         return;
//     QPixmap pix;
//     bool ret =pix.load(PressImgPath);

//     if(!ret){
//         qDebug() << "加载失败";
//         return;
//     }//加载图片

//     is_chosen=true;

//     this->resize(pix.width()*scale,pix.height()*scale);

//     this->setStyleSheet("QPushButton {border:0px;}");//设置不规则样式

//     this->setIcon(pix);

//     this->setIconSize((QSize(pix.width()*scale,pix.height()*scale)));

// }


// void MyPushButton::nchosen(){
//     if(!is_chosen)
//         return;

//     QPixmap pix;
//     bool ret =pix.load(normalImgPath);

//     if(!ret){
//         qDebug() << "加载失败";
//         return;
//     }//加载图片

//     is_chosen=false;

//     this->resize(pix.width()*scale,pix.height()*scale);

//     this->setStyleSheet("QPushButton {border:0px;}");//设置不规则样式

//     this->setIcon(pix);

//     this->setIconSize((QSize(pix.width()*scale,pix.height()*scale)));
// }


