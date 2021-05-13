#include "mainscene.h"
#include "ui_mainscene.h"
#include <QPainter>
#include "mypushbutton.h"
#include <QTimer>
#include <QSound>//需要在.pro中添加 multimedia.  (QT       += core gui multimedia)

MainScene::MainScene(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainScene)
{
    ui->setupUi(this);
    //配置主场景
    //设置固定大小
    setFixedSize(320,588);
    //设置图标
    setWindowIcon(QIcon(":/res/Coin0001.png"));
    //设置标题
    setWindowTitle("翻金币主场景");

    //退出按钮的实现
    connect(ui->actionquit,&QAction::triggered,[=](){
        this->close();
    });

    //开始按钮的音效
    QSound *startSound = new QSound(":/res/TapButtonSound.wav",this);


    //开始按钮
    MyPushButton *startBtn = new MyPushButton(":/res/MenuSceneStartButton.png");
    startBtn->setParent(this);
    startBtn->move(this->width()*0.5 - startBtn->width()*0.5,this->height()*0.7);

    //实例化选择关卡的场景
    chooseScene = new ChooseLevelScene();

    //监听返回按钮信号（连接只需要做一次即可）
    connect(chooseScene,&ChooseLevelScene::chooseSceneBack,this,[=](){
        QTimer::singleShot(500,this,[=](){
            //设置新旧场景在屏幕同一位置切换
            this->setGeometry(chooseScene->geometry());
            chooseScene->hide();
            this->show();
        });
    });

    connect(startBtn,&MyPushButton::clicked,[=](){
        startSound->play(); //开始音效
        //弹起特效
        startBtn->zoom1();
        //弹回特效
        startBtn->zoom2();
        //切换到选择关卡场景，隐藏自身，同时显示选择关卡场景,增加一个延时,0.5秒后让当前对象执行匿名函数
        QTimer::singleShot(500,this,[=](){
            //设置新旧场景位置一致
            chooseScene->setGeometry(this->geometry());
            this->hide();
            chooseScene->show();
        });

    });


}

void MainScene::paintEvent(QPaintEvent*)
{
    QPainter painter(this);
    QPixmap pix;
    pix.load(":/res/PlayLevelSceneBg.png");
    painter.drawPixmap(0,0,this->width(),this->height(),pix);
    //画背景上的图标
    pix.load(":/res/Title.png");
    pix.scaled(pix.width()*0.5,pix.height()*0.5);
    painter.drawPixmap(10,30,pix);
}

MainScene::~MainScene()
{
    delete ui;
}

