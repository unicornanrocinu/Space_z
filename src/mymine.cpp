#include "mymine.h"

myMine::myMine()
{
}


mine1::mine1(int p_x, int p_y,int cell_size,int v){
    myMine::p_x=p_x;
    myMine::p_y=p_y;
    type=1;
    divided=1;
    x=cell_size/2;
    y=cell_size/2;
    value=v;
}


mine2::mine2(int p_x, int p_y,int cell_size,int v){
    myMine::p_x=p_x;
    myMine::p_y=p_y;
    type=2;
    divided=0;
    x=cell_size/2;
    y=cell_size/2;
    value=v;
}



mine1s::mine1s(int p_x, int p_y,int cell_size,int v){
    myMine::p_x=p_x;
    myMine::p_y=p_y;
    type=3;
    divided=0;
    x=cell_size/2;
    y=cell_size/2;
    value=v;
}

mine1d::mine1d(int p_x, int p_y,int cell_size,int v){
    myMine::p_x=p_x;
    myMine::p_y=p_y;
    type=4;
    divided=0;
    x=cell_size/2;
    y=cell_size/2;
    value=v;
}
