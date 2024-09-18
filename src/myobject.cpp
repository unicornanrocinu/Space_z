#include "myobject.h"


QString trace::pix_n=":/res/turning_trace2.png";
QString trace::pix_s=":/res/turning_trace.png";

QString miner::pix_1=":/res/miner1_i.png";
QString miner::pix_2=":/res/miner2_i.png";

myObject::myObject(bool is_p):is_paint(is_p)
{

}


center::center(bool is_p,int size):myObject(is_p)
{
    pix=":/res/center_image.png";
    type=0;
    h=size;
    w=size;
}



trace::trace(int f,int c):myObject(1),is_curve(c)
{
    forward=f;
    pix=":/res/straight_trace.png";
    type=1;
    h=1;
    w=1;
}


miner::miner(int f):myObject(1){
    forward=f;
    pix=":/res/miner.png";
    type=2;
    h=1;
    w=1;
}


cut::cut(bool is_p,int f,bool is_i):myObject(is_p),is_intput(is_i){
    forward=f;
    pix=":/res/cut_machine.png";
    type=3;
    if(forward%2)
    {
        h=2;
        w=1;
    }
    else{
        h=1;
        w=2;
    }
}


bin::bin():myObject(1){
    forward=0;
    pix=":/res/rubbish_bin.png";
    type=4;
    h=1;
    w=1;
}
