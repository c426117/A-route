#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <queue>
#include <vector>

using namespace std;

#define W 15
#define H 15

enum Flag//节点状态
{
    closed,
    open,
    unvisit
};

struct Node//节点
{
    int x,y;
    int _G;
    int _H;
    int F;
    struct Node* pt;
};
struct Seal//Close表节点
{
    Flag flag;
    Node *point;
};



namespace Ui {
class MainWindow;
}



class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    void printmg ();
    ~MainWindow();


private slots:
    void on_pushButtonFile_clicked();

    void on_pushButtonSetOk_clicked();

    void on_pushButtonStart_clicked();

private:
    Ui::MainWindow *ui;
    int mg[H][W];
    Seal seal[H][W];//close表


public:
    int sx,sy,ex,ey;
    int direct[4][2] = {{0,1},{-1.0},{0,-1},{1,0}};//方向向量
    struct cmp
    {
        bool operator()(Node *n1,Node *n2)
        {
            return n1->F>n2->F;
        }
    };
    priority_queue<Node*,vector<Node*>,cmp> openque;//OPEN表，优先级队列
    int abs(int i )//求绝对值
    {
        return i>0?i:-i;
    }
    int getH (int x,int y)//求曼哈顿距离
    {
        return abs(x-ex)+abs(y-ey);
    }
    void A ();
    vector<Node*> Rout;
    void getRoute (Node *p);
    void printRout (vector<Node*> & Rout);

};

#endif // MAINWINDOW_H
