#ifndef MYPUSHBUTTON_H
#define MYPUSHBUTTON_H

#include <QPushButton>

class MyPushButton : public QPushButton
{
    Q_OBJECT
public:
    //explicit MyPushButton(QWidget *parent = nullptr);

    //构造函数
    MyPushButton(QString normalImg, QString pressImg="");

    //成员属性，保存用户传入的路径
    QString normalImgPath;
    QString pressImgPath;

    //弹跳特效
    void zoom1();
    void zoom2();

    //重写按钮 按下 和 释放 事件
    void mousePressEvent(QMouseEvent *e);

    void mouseReleaseEvent(QMouseEvent *e);

signals:

};

#endif // MYPUSHBUTTON_H
