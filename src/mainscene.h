#ifndef MAINSCENE_H
#define MAINSCENE_H

#include <QMainWindow>
#include <QPushButton>
#include "mapscene.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainScene;
}
QT_END_NAMESPACE

class MainScene : public QMainWindow
{
    Q_OBJECT

public:
    MainScene(QWidget *parent = nullptr);
    void paintEvent(QPaintEvent *event);
    ~MainScene();


private:
    Ui::MainScene *ui;
    mapscene* map_p;
    int money;
    int value_1;
    int value_2;
    int value_1s;
};


#endif // MAINSCENE_H
