#ifndef MAPSCENE_H
#define MAPSCENE_H

#include <QMainWindow>
#include <QKeyEvent>
#include <QPainter>
#include <QVector>
#include "myobject.h"
#include "mymine.h"

class mapscene : public QMainWindow
{
    Q_OBJECT
public:
    explicit mapscene(QWidget *parent);
    void closeEvent(QCloseEvent *event) override; // 重写closeEvent函数


    void paintEvent(QPaintEvent *event) override;


    void keyPressEvent(QKeyEvent *event) override;

    void mousePressEvent(QMouseEvent* event) override ;

    void mouseMoveEvent(QMouseEvent* event) override;

    void mouseReleaseEvent(QMouseEvent *event) override;//为实现鼠标在按压状态下移动自动放置trace（直）

    bool outmap(int p_x,int p_y);//检测是否超出范围

    bool candisplay(int p_x,int p_y);//封装outmap使其能够检验多格器件(名字取反了）

    bool delete_object(int p_x,int p_y);//删除map上的元素

    void mine_init();//矿产生成

    void add_mine(int type,int x,int y);//对于一个miner执行生成mine的操作

    void all_add();//对于全图的检索

    void move_mine(int p_x,int p_y);//单个格子mine的移动

    void move_all();//检索全图

    void cut_all();

    void clear_mine(int p_x,int p_y);//清除相应格子内的所有mine

    bool bump_mine_x(myMine* mine1,myMine* mine2);

    bool bump_mine_y(myMine* mine1,myMine* mine2);

    void boundary_judgment(int p_x,int p_y,int& i,int f);//判断mine跨越边界时的逻辑



private:
    int viewportX, viewportY; // 视口位置
    int map_size=50; //地图大小
    int cell_size=50;//格子大小
    int center_size=4;//交付中心大小
    int margin=50;  //地图边界的距离

    int money=9999;//金币数量

    int value_1=1;

    int value_2=2;

    int value_1s=2;


    int mouse_x,mouse_y;//记录鼠标位置


    // bool clicked_l=0; //记录鼠标左键按压状态 待实现

    bool clicked_r=0; //记录鼠标右键按压状态

    int choice=0;//当前选中的工具，0为无，1为trace，2为miner，3为cut，4为bin

    int forword=0;//记录当前工具的方向，0为上，1为右，2为下，3为左

    int is_curve=0;//记录trace的曲直 1为顺时针，2为逆时针

    myObject* map_store[50][50];

    int mine[50][50];//记录矿产分布 0为无 1为mine1 2为mine2

    QVector<myMine*> mines[50][50];// 记录每个格子中的开采矿物

    int add_speed=1000;//开采速率

    int move_speed=18;//传输速率

    int cut_speed=72;//切割速率

    int add_time=0;//开采进度

    int move_time=0;//传输进度

    int cut_time=0;//切割进度

    int mine1_num=0;//矿的交付数量

    int mine2_num=0;

    int mine1s_num=0;

    int mine1d_num=0;


signals:
    void windowClosed();
};



#endif // MAPSCENE_H
