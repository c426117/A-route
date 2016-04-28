#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QFile>
#include <QIODevice>
#include <QTextStream>
#include <QObject>
#include <QDebug>



MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->pushButtonStart->setDisabled(true);
    ui->pushButtonSetOk->setDisabled(true);
    ui->spinBoxSx->setRange(1,W-2);
    ui->spinBoxSx->setValue(1);
    ui->spinBoxEx->setRange(1,W-2);
    ui->spinBoxEx->setValue(W-2);
    ui->spinBoxSy->setRange(1,H-2);
    ui->spinBoxSy->setValue(1);
    ui->spinBoxEy->setRange(1,H-2);
    ui->spinBoxEy->setValue(H-2);
    ex=-1;
    ey=-1;
    sx=-1;
    sy=-1;

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::printmg()
{

    QString str;
    for(int i = 0;i!=H;i++)
    {
        for(int j=0;j!=W;j++)
        {
            int n = mg[i][j];
            if((i==ey)&&(j==ex))
            {
                str.append("★");
            }
            else if((i==sy)&&(j==sx))
            {
                str.append("☆");
            }
            else if(n==0)
            {
                str.append("□");
            }
            else if(n==1)
            {
                str.append("■");
            }

        }
        str.append("\n");
    }
    QFont font;
    font.setPixelSize(15);
    ui->textBrowserUi->setFont(font);
    ui->textBrowserUi->setText(str);
}

void MainWindow::printRout(vector<Node *> & Rout)
{
    bool p=true;
    QString str;
    for(int i = 0;i!=H;i++)
    {
        for(int j=0;j!=W;j++)
        {
            int n = mg[i][j];
            for(vector<Node*>::iterator it = Rout.begin()+1;it!=Rout.end();it++)
            {
                if((i==(*it)->y)&&(j==(*it)->x))
                {
                    str.append("●");
                    p=false;
                }
            }
            if((i==ey)&&(j==ex))
            {
                str.append("★");
            }
            else if((i==sy)&&(j==sx))
            {
                str.append("☆");
            }
            else if(n==0 && p)
            {
                str.append("□");
            }
            else if(n==1)
            {
                str.append("■");
            }
            p=true;
        }
        str.append("\n");
    }
    ui->textBrowserUi->setText(str);

}




void MainWindow::on_pushButtonFile_clicked()
{
    QFileDialog* FileChose= new QFileDialog(this);
    FileChose->setDirectory("D:");
    QString filename;
    filename = FileChose->getOpenFileName();
    delete(FileChose);


    QFile file(filename);
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    QTextStream in(&file);
    char buf;
    for(int i = 0 ; i<H;i++)
    {
        for(int j = 0;j<W;j++)
        {
            in>>buf;
            if(buf=='\n')
            {
                in>>buf;
            }
            mg[i][j]=(int)(buf-'0');
            seal[i][j].flag = unvisit;
            seal[i][j].point = NULL;
           //ui->textBrowserPrint->append(QString::number(mg[i][j]));
        }
    }
    ui->pushButtonSetOk->setEnabled(true);
    printmg();
}

void MainWindow::A()
{
    //原点加入open表
    Node *pnode = new Node;
    pnode->pt=NULL;
    pnode->_H=getH(sx,sy);
    pnode->_G=0;
    pnode->x=sx;
    pnode->y=sy;
    pnode->F=pnode->_H+pnode->_G;//优先级
    openque.push(pnode);
    seal[sy][sx].flag = open;
    seal[sy][sx].point = pnode;
    int testx=0;
    bool stopflag = true;
    while((!openque.empty())&&stopflag)
    {
        testx++;
        qDebug()<<testx;
        //sleep(1000);
        //将当前的节点加入到close表
        pnode=openque.top();
        //printPlace(pnode->x,pnode->y);
        openque.pop();
        int x = pnode->x;
        int y = pnode->y;
        seal[y][x].flag = closed;

        //搜索当前结点的四个方向
        for(int i =0;i<4;i++)
        {

            int tx = x+direct[i][0];
            int ty = y+direct[i][1];

            if(mg[ty][tx]==1 || seal[ty][tx].flag==closed)//当这个节点在close表中或者是障碍物
            {
                continue;
            }

            if(seal[ty][tx].flag==unvisit)
            {
                if(tx==ex && ty==ey)
                {
                    ui->textBrowserPrint->append("终点");
                    ui->textBrowserPrint->append("步数：");
                    ui->textBrowserPrint->append(QString::number(pnode->F));
                    getRoute(pnode);
                    printRout(Rout);
                    stopflag=false;
                    qDebug()<<"stopsdsdsds";
                }

                Node* temp = new Node;
                seal[ty][tx].flag=open;
                seal[ty][tx].point=temp;
                temp->pt = pnode;
                temp->x=tx;
                temp->y=ty;
                temp->_H = getH(tx,ty);
                temp->_G=pnode->_G+1;
                temp->F=temp->_G+temp->_H;
                openque.push(temp);
                //若是未访问过节点，且不是障碍物也不是终点
                //将这个节点加入到open表中并计算GHF值
                //链接它和其父节点
            }
            else
            {
                Node* temp = seal[ty][tx].point;
                if(pnode->_G+1 < temp->_G)
                {
                    temp->_G = pnode->_G+1;
                    temp->pt=pnode;
                    temp->F = temp->_G+temp->_H;
                }
            }
        }
        //ui->textBrowserPrint->append("无路径");
    }

}

void MainWindow::on_pushButtonSetOk_clicked()
{
    ui->pushButtonStart->setEnabled(true);
    sx=ui->spinBoxSx->value();
    sy=ui->spinBoxSy->value();
    ex=ui->spinBoxEx->value();
    ey=ui->spinBoxEy->value();
    printmg();
}

void MainWindow::getRoute(Node* p)
{
    if(p==NULL)
    {
        return;
    }
    getRoute(p->pt);
    Rout.push_back(p);
}

void MainWindow::on_pushButtonStart_clicked()
{
    ui->pushButtonStart->setDisabled(true);
    A();
}
