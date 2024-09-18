#ifndef MYPUSHBUTTON_H
#define MYPUSHBUTTON_H

#include <QPushButton>

class MyPushButton : public QPushButton
{
    Q_OBJECT
public:
    MyPushButton(QWidget *parent,QString normalImg,QString pressImg=" ",double scale=1);

    QString normalImgPath;
    QString PressImgPath;
    int scale;
    bool is_chosen;

    void chosen();
    void nchosen();

signals:
};


#endif // MYPUSHBUTTON_H
