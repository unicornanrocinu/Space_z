#ifndef MYMINE_H
#define MYMINE_H

#include <QObject>

class myMine : public QObject
{
    Q_OBJECT
public:
    myMine();
    int p_x,p_y; //mine的位置
    int x,y;//相对格子的坐标
    int type;//mine的类别
    bool divided;//是否可切割
    bool skip=0;//防止跨越区域后动两次
    int value;
};


class mine1:public myMine{
public:
    mine1(int p_x,int p_y,int cell_size,int v);
};


class mine2:public myMine{
public:
    mine2(int p_x,int p_y,int cell_size,int v);
};


class mine1s:public myMine{
public:
    mine1s(int p_x,int p_y,int cell_size,int v);
};

class mine1d:public myMine{
public:
    mine1d(int p_x,int p_y,int cell_size,int v);
};


#endif // MYMINE_H
