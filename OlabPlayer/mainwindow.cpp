#include "mainwindow.h"
#include "ui_mainwindow.h"

#define FRAMEINTERVAL 50.0                  //units are ms
#define FRAMEBOX_MAXNUM 10
#define FRAMEBOX_TTL (FRAMEINTERVAL*100)    //time to live, units are ms


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    timer.setInterval(FRAMEINTERVAL);
    connect(&timer,SIGNAL(timeout()),this,SLOT(refresh()));
    timer.start();
}

MainWindow::~MainWindow()
{
    delete ui;
    //if exit with frame servive
    for(int i=0;i<frameBoxLife.size();i++)
        delete frameBox[i];
}

void MainWindow::refresh()
{
    QString strTotalBoxStaus;
    for(int i=0;i<frameBoxLife.size();i++)
    {
        QPoint pos = (ui->radioButton_start->pos() + (1.0-(frameBoxLife[i]/FRAMEBOX_TTL)) * (ui->radioButton_end->pos()-ui->radioButton_start->pos()));
        frameBox[i]->move(pos);
        strTotalBoxStaus+=QString::number(frameBoxLife[i]);
        strTotalBoxStaus+=' ';
        frameBoxLife[i] -= FRAMEINTERVAL;
        if(frameBoxLife[i] <= 0)
        {
            frameBoxLife.remove(i);
            delete frameBox[i];
            frameBox.remove(i);
        }
    }
    qDebug() << strTotalBoxStaus;
}

void MainWindow::on_pushButton_snapshot_clicked()
{
    if (frameBoxLife.size()<FRAMEBOX_MAXNUM)
    {
        int ind=frameBoxLife.size();
        frameBoxLife << FRAMEBOX_TTL;
        frameBox << new QLabel(QString::number(ind),this);
        frameBox[ind]->move(ui->radioButton_start->pos());
        frameBox[ind]->setFrameShape(QFrame::Panel);
        frameBox[ind]->setFrameShadow(QFrame::Raised);
        frameBox[ind]->show();
    }

}
