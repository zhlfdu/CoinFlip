#include "chooselevelscene.h"
#include <QMenuBar>
#include <QPainter>
#include "mypushbutton.h"
#include <QLabel>
#include <QDebug>
#include <QTimer>
#include <QSound>

ChooseLevelScene::ChooseLevelScene(QWidget *parent) : QMainWindow(parent)
{
    //配置选择关卡场景信息
    this->setFixedSize(320,588);
    //设置图标
    this->setWindowIcon(QIcon(":/res/Coin0001.png"));
    //设置标题
    this->setWindowTitle("选择关卡");
    //由于没创建ui界面，在这里用代码写
    //创建菜单栏
    QMenuBar *bar = menuBar();
    setMenuBar(bar);
    //创建开始菜单
    QMenu *startMenu = bar->addMenu("开始");
    //创建退出菜单项
    QAction *quitAction = startMenu->addAction("退出");
    //信号槽连接，点击 退出 关闭窗口
    connect(quitAction,&QAction::triggered,[=](){
        this->close();
    });

    //选择关卡音效
    QSound *chooseSound = new QSound(":/res/TapButtonSound.wav",this);
    //返回按钮音效
    QSound *backSound = new QSound(":/res/BackButtonSound.wav",this);



    //返回按钮
    MyPushButton *backBtn = new MyPushButton(":/res/BackButton.png",":/res/BackButtonSelected.png");
    //放置到窗口中合适的位置
    backBtn->setParent(this);
    backBtn->move(this->width()-backBtn->width(),this->height()-backBtn->height());
    //实现点击按钮切换到主场景
    connect(backBtn,&QPushButton::clicked,[=](){
        backSound->play();
        //主场景要监听返回按钮的点击信号
        emit this->chooseSceneBack();
    });

    //创建20个关卡的按钮,利用一个for循环生成矩阵，使用%和/来实现
    for(int i=0;i<20;i++)
    {
        MyPushButton *menuBtn = new MyPushButton(":/res/LevelIcon.png");
        menuBtn->setParent(this);
        menuBtn->move(25+i%4*70, 130+i/4*70);

        //监听按钮点击事件
        connect(menuBtn,&MyPushButton::clicked,[=](){
            chooseSound->play();
            qDebug()<<"选择来第"<<i+1<<"关";
            //进入到对应关卡

            this->hide();//隐藏选关场景
            play = new PlayScene(i+1);//创建对应关卡场景
            play->setGeometry(this->geometry());
            play->show();//显示关卡

            //监听关卡场景中返回按钮信号
            connect(play,&PlayScene::chooseSceneBack,this,[=](){
                QTimer::singleShot(500,this,[=](){
                    this->setGeometry(play->geometry());
                    this->show();
                    delete play;
                    play=nullptr;
                });
            });
        });




        //设置关卡按钮上的文字
        QLabel *label =new QLabel;
        label->setParent(this);
        label->setFixedSize(menuBtn->width(),menuBtn->height());
        label->setText(QString::number(i+1));
        label->move(25+i%4*70, 130+i/4*70);
        label->setAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
        //设置让鼠标点击信号能穿透label，传递给menuBtn
        label->setAttribute(Qt::WA_TransparentForMouseEvents);
    }

}



void ChooseLevelScene::paintEvent(QPaintEvent*)
{
    QPainter painter(this);
    QPixmap pix;
    pix.load(":/res/OtherSceneBg.png");
    //参数为：起始坐标，绘画宽度，高度，素材
    painter.drawPixmap(0,0,this->width(),this->height(),pix);
    //再把标题画上
    pix.load(":/res/Title.png");
    painter.drawPixmap((this->width()-pix.width())*0.5,30,pix);
}
