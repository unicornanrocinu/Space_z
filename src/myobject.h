#ifndef MYOBJECT_H
#define MYOBJECT_H

#include <QObject>

class myObject
{
public:
    myObject(bool is_p);
    int forward=0;//0为上，1为右，2为下，3为左
    int type;//0为center,1为trace，2为miner，3为cut，4为bin
    bool is_paint;
    int w;
    int h;

    QString pix;
};

#endif // MYOBJECT_H


class center:public myObject{
public:
    center(bool is_p,int size);//标记主格

};


class trace:public myObject{
public:
    trace(int f,int c);//方向，弯直

    int is_curve=0;//是否为弯的路径 1为顺时针 2为逆时针


    static QString pix_s;//顺时针路径

    static QString pix_n;//逆时针路径
};


class miner:public myObject{
public:
    miner(int f);

    static QString pix_1;//矿1的图片
    static QString pix_2;//矿2的图片
};


class cut:public myObject
{
public:
    cut(bool is_p,int f,bool is_i);
    bool is_intput;//判断输入口
};

class bin:public myObject{
public:
    bin();
};
