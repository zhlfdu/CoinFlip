#ifndef PLAYSCENE_H
#define PLAYSCENE_H

#include <QMainWindow>
#include "mycoin.h"

class PlayScene : public QMainWindow
{
    Q_OBJECT
public:
    //explicit PlayScene(QWidget *parent = nullptr);
    //提供构造函数
    PlayScene(int levelNum);
    int levelIndex;

    //重写绘画事件，填充背景
    void paintEvent(QPaintEvent *);

    //维护每关的具体数据
    int gameArray[4][4];

    MyCoin * coinBtn[4][4]; //金币按钮数组

    bool isWin; //是否胜利

signals:
    void chooseSceneBack();
};

#endif // PLAYSCENE_H
