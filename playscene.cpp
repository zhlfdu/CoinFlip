#include "playscene.h"
#include <QMenuBar>
#include <QPainter>
#include "mypushbutton.h"
#include <QLabel>
#include "dataconfig.h"
#include "mycoin.h"
#include <QTimer>
#include <QPropertyAnimation>
#include <QSound>


/*
PlayScene::PlayScene(QWidget *parent) : QMainWindow(parent)
{

}
*/

PlayScene::PlayScene(int levelNum)
{
    this->levelIndex=levelNum;

    //配置选择关卡场景信息
    this->setFixedSize(320,588);
    //设置图标
    this->setWindowIcon(QIcon(":/res/Coin0001.png"));
    //设置标题
    this->setWindowTitle("翻金币");
    //由于没创建ui界面，在这里用代码写
    //创建菜单栏
    QMenuBar *bar = menuBar();
    setMenuBar(bar);
    //创建开始菜单
    QMenu *startMenu = bar->addMenu("开始");
    //创建退出菜单项
    QAction *quitAction = startMenu->addAction("退出");

    //音效
    //返回按钮音效
    QSound *backSound = new QSound(":/res/BackButtonSound.wav",this);
    //翻金币音效
    QSound *flipSound = new QSound(":/res/ConFlipSound.wav",this);
    //胜利按钮音效
    QSound *winSound = new QSound(":/res/LevelWinSound.wav",this);



    //返回按钮
    MyPushButton *backBtn = new MyPushButton(":/res/BackButton.png",":/res/BackButtonSelected.png");
    //放置到窗口中合适的位置
    backBtn->setParent(this);
    backBtn->move(this->width()-backBtn->width(),this->height()-backBtn->height());
    //实现点击按钮返回选关场景
    connect(backBtn,&QPushButton::clicked,[=](){
        //选关场景要监听返回按钮的点击信号
        backSound->play();
        emit this->chooseSceneBack();
    });

    //显示关卡名称
    QLabel *label = new QLabel;
    label->setParent(this);
    QFont font;
    font.setFamily("华文隶书");
    font.setPointSize(20);
    QString str= QString("Level:%1").arg(this->levelIndex);
    label->setFont(font);
    label->setText(str);
    label->setGeometry(30,this->height()-50,120,50);


    //初始化每一关的硬币显示
    dataConfig config;
    for(int i = 0 ; i < 4;i++)
    {
        for(int j = 0 ; j < 4; j++)
        {
            gameArray[i][j] = config.mData[this->levelIndex][i][j];
        }
    }


    QLabel* winLabel = new QLabel;
    QPixmap tmpPix;
    tmpPix.load(":/res/LevelCompletedDialogBg.png");
    winLabel->setGeometry(0,0,tmpPix.width(),tmpPix.height());
    winLabel->setPixmap(tmpPix);
    winLabel->setParent(this);
    winLabel->move( (this->width() - tmpPix.width())*0.5 , -tmpPix.height());



    //创建金币的背景图片
    for(int i = 0 ; i < 4;i++)
    {
        for(int j = 0 ; j < 4; j++)
        {
            //绘制背景图片
            QLabel* label = new QLabel;
            label->setGeometry(0,0,50,50);
            label->setPixmap(QPixmap(":/res/BoardNode.png"));
            label->setParent(this);
            label->move(57 + i*50,200+j*50);

            //创建金币
            QString str;
            if(this->gameArray[i][j] == 1)
            {
                str=":/res/Coin0001.png";
            }else
            {
                str=":/res/Coin0008.png";
            }
            MyCoin *coin= new MyCoin(str);
            coin->setParent(this);
            coin->move(59 + i*50,204+j*50);

            coin->posX = i; //记录x坐标
            coin->posY = j; //记录y坐标
            coin->flag =this->gameArray[i][j]; //记录正反标志

            coinBtn[i][j] = coin;
            //点击金币进行翻转
            connect(coin,&MyCoin::clicked,[=](){
                coin->changeFlag();
                this->gameArray[i][j] = this->gameArray[i][j] == 0? 1:0;
                flipSound->play();
                //翻转周围硬币
                QTimer::singleShot(300, this,[=](){
                    if(coin->posX+1 <=3)
                    {
                        coinBtn[coin->posX+1][coin->posY]->changeFlag();
                        gameArray[coin->posX+1][coin->posY] = gameArray[coin->posX+1][coin->posY]== 0 ? 1 : 0;
                    }
                    if(coin->posX-1>=0)
                    {
                        coinBtn[coin->posX-1][coin->posY]->changeFlag();
                        gameArray[coin->posX-1][coin->posY] = gameArray[coin->posX-1][coin->posY]== 0 ? 1 : 0;
                    }
                    if(coin->posY+1<=3)
                    {
                        coinBtn[coin->posX][coin->posY+1]->changeFlag();
                        gameArray[coin->posX][coin->posY+1] = gameArray[coin->posX+1][coin->posY]== 0 ? 1 : 0;
                    }
                    if(coin->posY-1>=0)
                    {
                        coinBtn[coin->posX][coin->posY-1]->changeFlag();
                        gameArray[coin->posX][coin->posY-1] = gameArray[coin->posX+1][coin->posY]== 0 ? 1 : 0;
                    }

                    //判断是否胜利
                    this->isWin = true;
                    for(int i = 0 ; i < 4;i++)
                    {
                        for(int j = 0 ; j < 4; j++)
                        {
                            //qDebug() << coinBtn[i][j]->flag ;
                            if( coinBtn[i][j]->flag == false)
                            {
                                this->isWin = false;
                                break;
                            }
                        }
                    }
                    if(this->isWin==true)
                    {
                        qDebug() << "胜利";
                        //所有按钮胜利标志改为true
                        for(int i = 0 ; i < 4;i++)
                        {
                            for(int j = 0 ; j < 4; j++)
                            {
                                coinBtn[i][j]->isWin=true;
                            }
                        }
                        winSound->play();//胜利音效
                        //弹出胜利图片
                        QPropertyAnimation * animation1 =  new QPropertyAnimation(winLabel,"geometry");
                        animation1->setDuration(1000);
                        animation1->setStartValue(QRect(winLabel->x(),winLabel->y(),winLabel->width(),winLabel->height()));
                        animation1->setEndValue(QRect(winLabel->x(),winLabel->y()+114,winLabel->width(),winLabel->height()));
                        animation1->setEasingCurve(QEasingCurve::OutBounce);
                        animation1->start();


                    }

                });






            });

        }
    }






}

void PlayScene::paintEvent(QPaintEvent *)
{
    //创建背景
    QPainter painter(this);
    QPixmap pix;
    pix.load(":/res/PlayLevelSceneBg.png");
    painter.drawPixmap(0,0,this->width(),this->height(),pix);

    //画标题
    pix.load(":/res/Title.png");
    pix.scaled(pix.width()*0.5,pix.height()*0.5);
    painter.drawPixmap(10,30,pix);
}
