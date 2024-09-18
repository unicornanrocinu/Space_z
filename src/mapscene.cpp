#include "mapscene.h"
#include <QTimer>
#include <QDebug>
#include <QtCore>   //生成随机数的头文件
#include <cstdlib>  //绝对值函数
#include "mypushbutton.h"

mapscene::mapscene(QWidget *parent)
    : QMainWindow{parent},viewportX(0), viewportY(0)
{   //基础页面设置
    this->setFixedSize(parent->width(),parent->height());
    setWindowIcon(QIcon(":/res/icon.png"));
    setWindowTitle(QString("异形工场"));

    //初始显示在屏幕中央
    viewportX=-cell_size*map_size/2+this->width()/2;
    viewportY=-cell_size*map_size/2+this->height()/2;

    //创建数组(最大的地图)储存地图格子信息并初始化
    for (auto &x:map_store)
        for (auto &y:x)
            y=nullptr;

    int center_positionX=map_size/2-center_size/2;
    int center_positionY=map_size/2-center_size/2;

    for(int i=0;i<center_size;i++)
        for(int j=0;j<center_size;j++)
        {
            if(i==0&&j==0)
                map_store[center_positionX+i][center_positionY+j]=new center(1,center_size);
            else
                map_store[center_positionX+i][center_positionY+j]=new center(0,center_size);
        }

    //退出按钮
    MyPushButton *exit_btn=new MyPushButton(this,":/res/exit_button.png","",0.5);
    exit_btn->setParent(this);
    exit_btn->move(this->width()-exit_btn->width(),0);
    connect(exit_btn,MyPushButton::clicked,[&](){
        this->close();
    });

    //帮助按钮
    MyPushButton *help_btn=new MyPushButton(this,":/res/help_button.png");
    help_btn->setParent(this);
    help_btn->move(0,this->height()-help_btn->height());

    //放置工具按钮
    MyPushButton *straight_trace_btn=new MyPushButton(this,":/res/trace_btn_p.png",":/res/trace_btn2.png",0.6);
    straight_trace_btn->setParent(this);
    straight_trace_btn->move(0.21*this->width(),this->height()-straight_trace_btn->height()-5);
    connect(straight_trace_btn,MyPushButton::clicked,[&](){
        if(choice==1)
            choice=0;
        else
            choice=1;
        forword=0;
        is_curve=0;
        });


    MyPushButton *miner_btn=new MyPushButton(this,":/res/miner_btn_p.png",":/res/miner_btn2.png",0.6);
    miner_btn->setParent(this);
    miner_btn->move(0.31*this->width(),this->height()-miner_btn->height()-5);
    connect(miner_btn,MyPushButton::clicked,[&](){
        if(choice==2)
            choice=0;
        else
            choice=2;
        forword=0;
    });


    MyPushButton *cut_btn=new MyPushButton(this,":/res/cut_btn_p.png",":/res/cut_btn2.png",0.6);
    cut_btn->setParent(this);
    cut_btn->move(0.41*this->width(),this->height()-cut_btn->height()-5);
    connect(cut_btn,MyPushButton::clicked,[&](){
        if(choice==3)
            choice=0;
        else
            choice=3;
        forword=0;
    });


    MyPushButton *bin_btn=new MyPushButton(this,":/res/bin_btn_p.png",":/res/bin_btn2.png",0.58);
    bin_btn->setParent(this);
    bin_btn->move(0.51*this->width(),this->height()-bin_btn->height()-5);
    connect(bin_btn,MyPushButton::clicked,[&](){
        if(choice==4)
            choice=0;
        else
            choice=4;
        forword=0;
    });


    setMouseTracking(true); // 启用鼠标跟踪

    mine_init();//初始化矿产

    //刷新屏幕
    QTimer *game_frame= new QTimer(this);
    game_frame->setInterval(3);
    game_frame->start();
    connect(game_frame,QTimer::timeout,this,[&](){
        add_time++;
        move_time++;
        cut_time++;
        if(add_time==add_speed){
            add_time=0;
            all_add();
        }
        if(move_time==move_speed){
            move_time=0;
            move_all();
        }
        if(cut_time==cut_speed){
            cut_time=0;
            cut_all();
        }
        update();
    });
}

void mapscene::closeEvent(QCloseEvent *event) {
    emit windowClosed();
    QWidget::closeEvent(event); // 继续默认的关闭处理
}

void mapscene::paintEvent(QPaintEvent *event) {
    QPainter painter(this);

    //绘制一个矩形来表示地图区域
    QPixmap pix;
    pix.load(":/res/background.png");
    painter.drawPixmap(0,0,this->width(),this->height(),pix);
    painter.drawRect(viewportX, viewportY, cell_size*map_size, cell_size*map_size);
    for(int i=0;i<=map_size;i++)
        painter.drawLine(QPoint(viewportX,viewportY+i*cell_size),QPoint(viewportX+cell_size*map_size,viewportY+i*cell_size));
    for(int i=0;i<=map_size;i++)
        painter.drawLine(QPoint(i*cell_size+viewportX,viewportY),QPoint(i*cell_size+viewportX,cell_size*map_size+viewportY));

    //绘制矿产
    for(int i=0;i<map_size;i++)
        for(int j=0;j<map_size;j++){
            if(mine[i][j]==0)
                continue;
            else if(mine[i][j]==1)
            {
                pix.load(":/res/miner1.png");
                painter.drawPixmap(viewportX+i*cell_size,viewportY+j*cell_size,cell_size,cell_size,pix);
            }
            else if(mine[i][j]==2)
            {
                pix.load(":/res/miner2.png");
                painter.drawPixmap(viewportX+i*cell_size,viewportY+j*cell_size,cell_size,cell_size,pix);
            }
        }

    //绘制轨道
    for(int i=0;i<map_size;i++)
        for(int j=0;j<map_size;j++)
        {
            bool flag=(map_store[i][j]==Q_NULLPTR);
            if(!flag)
            {
                if(map_store[i][j]->type==1)

                {
                    if(((trace*)map_store[i][j])->is_curve)
                    {
                        if(((trace*)map_store[i][j])->is_curve==1)
                            pix.load(trace::pix_s);
                        else
                            pix.load(trace::pix_n);
                    }
                    else
                        pix.load(map_store[i][j]->pix);
                    pix=pix.transformed(QTransform().rotate(90*map_store[i][j]->forward));//旋转图片
                    painter.drawPixmap(viewportX+i*cell_size,viewportY+j*cell_size,cell_size*map_store[i][j]->w,cell_size*map_store[i][j]->h,pix);
                }
            }
        }

    //绘制矿传送
    for(int i=0;i<map_size;i++)
        for(int j=0;j<map_size;j++)
        {
            if(!(mines[i][j].isEmpty()))
            {
                for(auto cell:mines[i][j]){
                    if(cell->type==1)
                        pix.load(":/res/mine1.png");
                    else if(cell->type==2)
                        pix.load(":/res/mine2.png");
                    else if(cell->type==3)
                    {
                        pix.load(":/res/mine1s.png");
                        pix=pix.transformed(QTransform().rotate(90*3));
                    }
                    else{
                        pix.load(":/res/mine1s.png");
                        pix=pix.transformed(QTransform().rotate(90));
                    }
                    painter.drawPixmap(viewportX+cell->x+cell->p_x*cell_size-0.4*cell_size,viewportY+cell->y+cell->p_y*cell_size-0.4*cell_size,0.8*cell_size,0.8*cell_size,pix);
                }
            }
        }


    //绘制器件(除轨道)
    for(int i=0;i<map_size;i++)
        for(int j=0;j<map_size;j++)
        {
            bool flag=(map_store[i][j]==Q_NULLPTR);
            if(!flag)
            {
                if(map_store[i][j]->is_paint)
                {
                    if(map_store[i][j]->type==2&&mine[i][j])
                    {
                        if(mine[i][j]==1)
                            pix.load(miner::pix_1);
                        else if(mine[i][j]==2)
                            pix.load(miner::pix_2);
                    }
                    else if(map_store[i][j]->type==1)
                        continue;
                    else
                        pix.load(map_store[i][j]->pix);
                    pix=pix.transformed(QTransform().rotate(90*map_store[i][j]->forward));//旋转图片
                    painter.drawPixmap(viewportX+i*cell_size,viewportY+j*cell_size,cell_size*map_store[i][j]->w,cell_size*map_store[i][j]->h,pix);
                }
            }
        }

    painter.save();

    //绘制当前器件交付数量
    painter.setPen(Qt::black);
    QFont font("Arial",6*center_size); // 字体为Arial，大小为16点
    painter.setFont(font);
    int center_positionX=map_size/2-center_size/2;
    int center_positionY=map_size/2-center_size/2;
    painter.drawText(viewportX+(center_positionX+0.3*center_size)*cell_size-0.15*center_size*cell_size,viewportY+(center_positionY+0.3*center_size)*cell_size,20*center_size,10*center_size,Qt::AlignCenter,QString::number(mine2_num));
    painter.drawText(viewportX+(center_positionX+0.6*center_size)*cell_size-0.15*center_size*cell_size,viewportY+(center_positionY+0.3*center_size)*cell_size,20*center_size,10*center_size,Qt::AlignCenter,QString::number(mine1_num));
    painter.drawText(viewportX+(center_positionX+0.3*center_size)*cell_size-0.15*center_size*cell_size,viewportY+(center_positionY+0.65*center_size)*cell_size,20*center_size,10*center_size,Qt::AlignCenter,QString::number(mine1s_num));
    painter.drawText(viewportX+(center_positionX+0.6*center_size)*cell_size-0.15*center_size*cell_size,viewportY+(center_positionY+0.65*center_size)*cell_size,20*center_size,10*center_size,Qt::AlignCenter,QString::number(mine1d_num));


    //画出工作区(圆角矩形）
    painter.setOpacity(0.5);
    painter.setBrush(QColor(128, 128, 128));
    painter.drawRoundedRect(0.2*this->width(),this->height()-80,0.6*this->width(),80,20,20);


    painter.restore();

    // 设置选中区域图像
    QPen pen(Qt::green, 3); // 使用绿色画笔，线宽为2
    painter.setPen(pen);
    QPen pen_r(Qt::red, 3);

    if(choice){
        painter.drawRect((0.107+0.1*choice)*this->width(),this->height()-80,80,80);
        int p_x=(mouse_x-viewportX)/cell_size;
        int p_y=(mouse_y-viewportY)/cell_size;
        //设置半透明图像

        switch(choice){
        case 1:
            if(is_curve==0)
            {
                pix.load(":/res/straight_trace.png");
            }
            else if(is_curve==1){
                pix.load(trace::pix_s);
            }
            else
            {
                pix.load(trace::pix_n);
            }
            break;
        case 2:
            pix.load(":/res/miner.png");
            break;
        case 3:
            pix.load(":/res/cut_machine.png");
            break;
        case 4:
            pix.load(":/res/rubbish_bin.png");
            break;
        }


        painter.setOpacity(0.5);
        if(choice!=4)//bin不旋转
        pix=pix.transformed(QTransform().rotate(90*forword));//旋转图片
        if(choice!=3)
        painter.drawPixmap(viewportX+p_x*cell_size,viewportY+p_y*cell_size,cell_size,cell_size,pix);
        else
        {
            int h,w;
            int p_x2,p_y2;
            switch(forword){
            case 0:
                h=1;
                w=2;
                p_x2=p_x;
                p_y2=p_y;
                break;

            case 1:
                h=2;
                w=1;
                p_x2=p_x;
                p_y2=p_y;
                break;

            case 2:
                h=1;
                w=2;
                p_x2=p_x-1;
                p_y2=p_y;
                break;

            case 3:
                h=2;
                w=1;
                p_x2=p_x;
                p_y2=p_y-1;
                break;
            }
        painter.drawPixmap(viewportX+p_x2*cell_size,viewportY+p_y2*cell_size,cell_size*w,cell_size*h,pix);
        }
        painter.setOpacity(1);


        //设置选中边框 红为不能安置，绿为可以
        if(candisplay(p_x,p_y))
            painter.setPen(pen_r);
        //记录mousex,y所在格子位置
        if(choice!=3){
            painter.drawRect(viewportX+p_x*cell_size,viewportY+p_y*cell_size,cell_size,cell_size);
        }
        else{
            if(forword==3){
                painter.drawRect(viewportX+p_x*cell_size,viewportY+(p_y-1)*cell_size,cell_size,cell_size*2);
            }
            else if(forword==0){
                painter.drawRect(viewportX+p_x*cell_size,viewportY+p_y*cell_size,cell_size*2,cell_size);
            }
            else if(forword==1){
                painter.drawRect(viewportX+p_x*cell_size,viewportY+p_y*cell_size,cell_size,cell_size*2);
            }
            else if(forword==2){
                painter.drawRect(viewportX+(p_x-1)*cell_size,viewportY+p_y*cell_size,cell_size*2,cell_size);
            }

        }//两格器件判断
    }

    //绘制金币界面
    pix.load(":/res/money.png");
    painter.drawPixmap(0,0,0.06*this->width(),0.12*this->height(),pix);
    painter.setPen(Qt::white);
    font.setPointSize(30);
    painter.setFont(font);
    painter.drawText(0.06*this->width(),0.06*this->height()-25,250,100,Qt::AlignLeft,QString::number(money));
}

void mapscene::keyPressEvent(QKeyEvent *event) {
    qDebug()<<event->key();
    if (event->key() == Qt::Key_D) {
        if(cell_size*map_size+viewportX+margin>=this->width())
            viewportX -= 20; // 向右移动视口
    } else if (event->key() == Qt::Key_A) {
        if(viewportX-margin<=0)
            viewportX += 20; // 向左移动视口
    } else if (event->key() == Qt::Key_S) {
        if(cell_size*map_size+viewportY+margin>=this->height())
            viewportY -= 20; // 向下移动视口
    } else if (event->key() == Qt::Key_W) {
        if(viewportY-margin<=0)
            viewportY += 20; // 向上移动视口
    } else if (choice&&event->key()==Qt::Key_R){
        forword++;
        forword%=4;
    } else if (choice==1&&event->key()==Qt::Key_Q){
        is_curve++;
        is_curve%=3;
    }
}

void mapscene::mousePressEvent(QMouseEvent* event){
    qDebug()<<"clicked";
    if(event->button()==Qt::RightButton)
    {
        clicked_r=true;
        if(choice)
            choice=0;
        else
        {
            int p_x=(mouse_x-viewportX)/cell_size;
            int p_y=(mouse_y-viewportY)/cell_size;
            if(!candisplay(p_x,p_y))
            {
                delete_object(p_x,p_y);
            }
        }
    }
    else if(choice&&event->button()==Qt::LeftButton){
        int p_x=(mouse_x-viewportX)/cell_size;
        int p_y=(mouse_y-viewportY)/cell_size;
        if(!candisplay(p_x,p_y))
        {
            delete_object(p_x,p_y);
            if(choice==1)// 创建trace
                {
                    trace* p=new trace(forword,is_curve);
                    map_store[p_x][p_y]=p;

                    //实现轨道建立的连贯性
                    switch(is_curve)
                    {
                    case 2:
                        forword+=2;
                    case 1:
                        forword+=1;
                    default:
                        break;
                    }

                    forword%=4;
                    is_curve=0;
                }
            else if(choice==2)//创建miner
                {
                miner* p=new miner(forword);
                map_store[p_x][p_y]=p;
                }
            else if(choice==3){
                    if(forword==0){
                        delete_object(p_x+1,p_y);
                        cut *p1=new cut(1,forword,1);//is_paint,is_input
                        cut *p2=new cut(0,forword,0);
                        map_store[p_x][p_y]=p1;
                        map_store[p_x+1][p_y]=p2;
                    }
                    else if(forword==1){
                        delete_object(p_x,p_y+1);
                        cut *p1=new cut(1,forword,1);//is_paint,is_input
                        cut *p2=new cut(0,forword,0);
                        map_store[p_x][p_y]=p1;
                        map_store[p_x][p_y+1]=p2;
                    }
                    else if(forword==2){
                        delete_object(p_x-1,p_y);
                        cut *p1=new cut(0,forword,1);//is_paint,is_input
                        cut *p2=new cut(1,forword,0);
                        map_store[p_x][p_y]=p1;
                        map_store[p_x-1][p_y]=p2;
                    }
                    else if(forword==3){
                        delete_object(p_x,p_y-1);
                        cut *p1=new cut(0,forword,1);//is_paint,is_input
                        cut *p2=new cut(1,forword,0);
                        map_store[p_x][p_y]=p1;
                        map_store[p_x][p_y-1]=p2;
                    }
                }
            else if(choice==4)//创建bin
                {
                bin* p=new bin();
                map_store[p_x][p_y]=p;
                }
        }
    }
    return QMainWindow::mousePressEvent(event);
}

void mapscene::mouseMoveEvent(QMouseEvent* event) {
    mouse_x=event->pos().x();
    mouse_y=event->pos().y();
    if(clicked_r)//实现滑动删除
    {
        int p_x=(mouse_x-viewportX)/cell_size;
        int p_y=(mouse_y-viewportY)/cell_size;
        if(!candisplay(p_x,p_y))
            delete_object(p_x,p_y);
    }
    return QMainWindow::mouseMoveEvent(event);
}

void mapscene::mouseReleaseEvent(QMouseEvent *event){
    if(event->button()==Qt::RightButton)
        clicked_r=false;
    return QMainWindow::mouseReleaseEvent(event);
}

bool mapscene::candisplay(int p_x,int p_y){
    if(choice!=3)
        return outmap(p_x,p_y);
    if(forword==3)
        return outmap(p_x,p_y)||outmap(p_x,p_y-1);
    else if(forword==0)
        return outmap(p_x,p_y)||outmap(p_x+1,p_y);
    else if(forword==1)
        return outmap(p_x,p_y)||outmap(p_x,p_y+1);
    if(forword==2)
        return outmap(p_x,p_y)||outmap(p_x-1,p_y);
    return 0;
}

bool mapscene::outmap(int p_x,int p_y){
    if(p_x<0||p_x>=map_size)
        return 1;
    if(p_y<0||p_y>=map_size)
        return 1;
    if(map_store[p_x][p_y]==nullptr)
        return 0;
    if(map_store[p_x][p_y]->type==0)
        return 1;
    return 0;
}

bool mapscene::delete_object(int p_x,int p_y){
    if(map_store[p_x][p_y]==nullptr)
        return 0;
    if(map_store[p_x][p_y]->type==0)
        return 0;
    if(map_store[p_x][p_y]->type!=3)
    {
        clear_mine(p_x,p_y);
        delete map_store[p_x][p_y];
        map_store[p_x][p_y]=nullptr;
        return 1;
    }
    if(((cut*)map_store[p_x][p_y])->is_intput){
        switch (map_store[p_x][p_y]->forward) {
        case 0:
            clear_mine(p_x+1,p_y);
            delete map_store[p_x+1][p_y];
            map_store[p_x+1][p_y]=nullptr;
            break;
        case 1:
            clear_mine(p_x,p_y+1);
            delete map_store[p_x][p_y+1];
            map_store[p_x][p_y+1]=nullptr;
            break;
        case 2:
            clear_mine(p_x-1,p_y);
            delete map_store[p_x-1][p_y];
            map_store[p_x-1][p_y]=nullptr;
            break;
        case 3:
            clear_mine(p_x,p_y-1);
            delete map_store[p_x][p_y-1];
            map_store[p_x][p_y-1]=nullptr;
            break;
        }
    }
    else{
        switch (map_store[p_x][p_y]->forward) {
        case 0:
            clear_mine(p_x-1,p_y);
            delete map_store[p_x-1][p_y];
            map_store[p_x-1][p_y]=nullptr;
            break;
        case 1:
            clear_mine(p_x,p_y-1);
            delete map_store[p_x][p_y-1];
            map_store[p_x][p_y-1]=nullptr;
            break;
        case 2:
            clear_mine(p_x+1,p_y);
            delete map_store[p_x+1][p_y];
            map_store[p_x+1][p_y]=nullptr;
            break;
        case 3:
            clear_mine(p_x,p_y+1);
            delete map_store[p_x][p_y+1];
            map_store[p_x][p_y+1]=nullptr;
            break;
        }
    }
    clear_mine(p_x,p_y);
    delete map_store[p_x][p_y];
    map_store[p_x][p_y]=nullptr;
    return 1;
}

void mapscene::mine_init(){
    for(int i=0;i<map_size;i++)
        for(int j=0;j<map_size;j++){
            int randomNumber = QRandomGenerator::global()->generate(); // 生成32位随机整数
            if(randomNumber%80==0)
                mine[i][j]=1;
            else if(randomNumber%80==1)
                mine[i][j]=2;
            else
                mine[i][j]=0;
            }
}


void mapscene::all_add(){
    for(int i=0;i<map_size;i++)
        for(int j=0;j<map_size;j++){
            if(map_store[i][j]==nullptr)
            continue;
            if(map_store[i][j]->type==2)
            {
                add_mine(mine[i][j],i,j);
            }
        }
}


void mapscene::add_mine(int type,int p_x,int p_y){
    if(!mines[p_x][p_y].isEmpty())
        return;
    if(type==0)//无矿
        return;
    if(type==1)
    {
        mine1* p=new mine1(p_x,p_y,cell_size,value_1);
        mines[p_x][p_y].append(p);
        qDebug()<<"add 1";
    }
    else if(type==2)
    {
        mine2* p=new mine2(p_x,p_y,cell_size,value_2);
        mines[p_x][p_y].append(p);
        qDebug()<<"add 2";
    }
}


void mapscene::move_mine(int p_x,int p_y){
    for(int i=0;i<mines[p_x][p_y].size();i++){
        int f=map_store[p_x][p_y]->forward;//记录其方向
        bool f_change=0;//防止多次修正方向
        myMine* &mine=mines[p_x][p_y][i];
        if(mine->skip){
            mine->skip=0;
            return;
        }
        if(f%2){
            if(mine->x==cell_size/2)
            {
                if(map_store[p_x][p_y]->type==1&&((trace*)map_store[p_x][p_y])->is_curve){
                    if(((trace*)map_store[p_x][p_y])->is_curve==1)
                        f++;
                    else
                        f+=3;
                    f%=4;
                    f_change=1;
                }
                else if(map_store[p_x][p_y]->type==3&&mine->divided){
                        continue;
                }
            }
        }
        else{
            if(mine->y==cell_size/2)
            {
                if(map_store[p_x][p_y]->type==1&&((trace*)map_store[p_x][p_y])->is_curve){
                    if(((trace*)map_store[p_x][p_y])->is_curve==1)
                        f++;
                    else
                        f+=3;
                    f%=4;
                    f_change=1;
                }
                else if(map_store[p_x][p_y]->type==3&&mine->divided){
                    continue;
                }
            }
        }
        int dx,dy;
        switch (f) {
        case 0:
            dx=0;
            dy=-1;
            break;
        case 1:
            dx=1;
            dy=0;
            break;
        case 2:
            dx=0;
            dy=1;
            break;
        case 3:
            dx=-1;
            dy=0;
            break;
        }
        if(i==0){
            if(!mines[p_x+dx][p_y+dy].isEmpty()){
                if(map_store[p_x][p_y]->type==1&&((trace*)map_store[p_x][p_y])->is_curve&&!f_change)
                        ;// 防止弯道堵塞
                else
                    {
                        if(dy)
                        {
                            if(bump_mine_y(mine,mines[p_x+dx][p_y+dy][mines[p_x+dx][p_y+dy].size()-1]))
                                return;
                        }
                        else{
                            if(bump_mine_x(mine,mines[p_x+dx][p_y+dy][mines[p_x+dx][p_y+dy].size()-1]))
                                return;
                        }
                    }
                }
        }
        else{
            if(dy)
            {
                if(bump_mine_y(mine,mines[p_x][p_y][i-1]))
                    return;
            }
            else
                if(bump_mine_x(mine,mines[p_x][p_y][i-1]))
                    return;
        }
        mine->x+=dx*2;
        mine->y+=dy*2;
        if(mine->x<0||mine->y<0||mine->x>cell_size||mine->y>cell_size)
            boundary_judgment(p_x,p_y,i,f);
    }
}


void mapscene::move_all(){
    for(int i=0;i<map_size;i++)
        for(int j=0;j<map_size;j++)
        {
            move_mine(i,j);
        }
}


void mapscene::clear_mine(int p_x,int p_y){
    for(auto &mine:mines[p_x][p_y])
    {
        delete mine;
    }
    mines[p_x][p_y].clear();
}


bool mapscene::bump_mine_x(myMine* mine1,myMine* mine2){
    if(std::abs((mine1->p_x*cell_size+mine1->x)-(mine2->p_x*cell_size+mine2->x))<0.8*cell_size)
        return 1;
    return 0;
}

bool mapscene::bump_mine_y(myMine* mine1,myMine* mine2){
    if(std::abs((mine1->p_y*cell_size+mine1->y)-(mine2->p_y*cell_size+mine2->y))<0.8*cell_size)
        return 1;
    return 0;
}


void mapscene::boundary_judgment(int p_x,int p_y,int& i,int f){
    int dx,dy;
    switch (f) {
    case 0:
        dx=0;
        dy=-1;
        break;
    case 1:
        dx=1;
        dy=0;
        break;
    case 2:
        dx=0;
        dy=1;
        break;
    case 3:
        dx=-1;
        dy=0;
        break;
    }
    auto &mine=mines[p_x][p_y][i];
    if(p_x+dx<0||p_y+dy<0||p_x+dx>=map_size||p_y+dy>=map_size)
    {
        mine->x-=dx*2;
        mine->y-=dy*2;
        return;
    }
    if(map_store[p_x+dx][p_y+dy]==nullptr||map_store[p_x+dx][p_y+dy]->type==2)//miner不接受输入
    {
        mine->x-=dx*2;
        mine->y-=dy*2;
        return;
    }
    if(map_store[p_x+dx][p_y+dy]->type==4)
    {
        delete mine;
        mines[p_x][p_y].remove(i);
        i--;
        return;
    }
    if(map_store[p_x+dx][p_y+dy]->type==0)
    {
        switch(mine->type){
        case 1:
            mine1_num++;
            break;
        case 2:
            mine2_num++;
            break;
        case 3:
            mine1s_num++;
            break;
        case 4:
            mine1d_num++;
            break;
        }
        money+=mine->value;
        delete mine;
        mines[p_x][p_y].remove(i);
        i--;
        return;
    }
    if(map_store[p_x+dx][p_y+dy]->forward!=f)//方向不同，暂停前进
    {
        mine->x-=dx*2;
        mine->y-=dy*2;
        return;
    }
    if(map_store[p_x+dx][p_y+dy]->type==3)//非输入端的cut输入或者非切割型mine
    {
        if(!(((cut*)map_store[p_x+dx][p_y+dy])->is_intput)||mine->divided==0)
        {
            mine->x-=dx*2;
            mine->y-=dy*2;
            return;
        }
    }
    mine->x-=cell_size*dx;
    mine->y-=cell_size*dy;
    mine->p_x+=dx;
    mine->p_y+=dy;
    if(dx>0||dy>0)
        mine->skip=1;
    mines[p_x+dx][p_y+dy].append(mines[p_x][p_y][i]);
    mines[p_x][p_y].remove(i);
    i--;
}


void mapscene::cut_all(){
    for(int i=0;i<map_size;i++)
        for(int j=0;j<map_size;j++){
            if(map_store[i][j]!=nullptr&&map_store[i][j]->type==3&&((cut*)map_store[i][j])->is_intput){
                if(!mines[i][j].isEmpty()&&mines[i][j][0]->x==mines[i][j][0]->y)
                {
                    int dx,dy;
                    switch(map_store[i][j]->forward){
                    case 0:
                        dx=1;
                        dy=0;
                        break;
                    case 1:
                        dx=0;
                        dy=1;
                        break;
                    case 2:
                        dx=-1;
                        dy=0;
                        break;
                    case 3:
                        dx=0;
                        dy=-1;
                        break;
                    }
                    if(mines[i+dx][j+dy].isEmpty())
                    {
                        if(map_store[i][j]->forward/2)
                        {
                            delete mines[i][j][0];
                            mines[i][j].remove(0);
                            mine1s* p1=new mine1s(i,j,cell_size,value_1s);
                            mines[i][j].append(p1);
                            mine1d* p2=new mine1d(i+dx,j+dy,cell_size,value_1s);
                            mines[i+dx][j+dy].append(p2);
                        }
                        else{
                            delete mines[i][j][0];
                            mines[i][j].remove(0);
                            mine1d* p1=new mine1d(i,j,cell_size,value_1s);
                            mines[i][j].append(p1);
                            mine1s* p2=new mine1s(i+dx,j+dy,cell_size,value_1s);
                            mines[i+dx][j+dy].append(p2);
                        }
                    }//确保另一个格子为空，防止阻塞
                }
            }
        }
}
