#ifndef CHOOSELEVELSCENE_H
#define CHOOSELEVELSCENE_H

#include <QMainWindow>
#include "playscene.h"


class ChooseLevelScene : public QMainWindow
{
    Q_OBJECT
public:
    explicit ChooseLevelScene(QWidget *parent = nullptr);

    //重新绘图事件来填充背景
    void paintEvent(QPaintEvent*);

    //创建关卡场景指针
    PlayScene *play = nullptr;

signals:
    //写一个自定义信号，告诉主场景点击了返回;自定义信号只需要声明，不需要实现，用emit发送
    void chooseSceneBack();

};

#endif // CHOOSELEVELSCENE_H
