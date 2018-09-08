/*
 * mainwindow.cpp
 *
 * Last UPdate: atian 2018/9/7
 *
 *
 */

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "cvmatandqimage.h"

#define FRAMEINTERVAL 40.0                  //units are ms, it change the target fps, 控制显示帧率！！只是显示！！

#define DEFAULT_VIDEO_COLS 640
#define DEFAULT_VIDEO_ROWS 480

#define MAT_BUF_SIZE 10

//#define PRINTDEBUG


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    //QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));
    ui->setupUi(this);
    timer.setInterval(FRAMEINTERVAL);
    connect(&timer,SIGNAL(timeout()),this,SLOT(refresh()));
    timer.start();

    timer_1s.setInterval(1000);
    connect(&timer_1s,SIGNAL(timeout()),this,SLOT(refresh_1s()));
    timer_1s.start();

    ui->label_video->setMinimumSize(DEFAULT_VIDEO_COLS,DEFAULT_VIDEO_ROWS);

    maxBufSize = MAT_BUF_SIZE;

    /******************************
     * Add Init here
     * ***************************/


    /*^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/
}

MainWindow::~MainWindow()
{
    //close first if open before
    if(matProThread.isRunning())
    {
        matProThread.stop();
        matProThread.wait();
    }
    delete ui;
    matProThread.stop();
}

/*
 * redefine the FRAMEINTERVAL to change the speed of refreash action
 *
 */
void MainWindow::refresh()
{

    if(ui->radioButton->isChecked())
    {
        matProThread.getOneMat();
        cv::Mat mat = matProThread.mat;
        if (!mat.empty())
        {
            matProThread.mutex.lock();
            mainVideo = QtOcv::mat2Image(mat);
            matProThread.mutex.unlock();
            ui->label_video->setPixmap(QPixmap::fromImage(mainVideo).scaled(ui->spinBox_cols->value(),ui->spinBox_rows->value()));
        }
    }
    else
        ui->label_video->setText("Proccing Background...");

    ui->statusBar->showMessage("||Buf of frames:"+QString::number(matProThread.matBuf.size())+\
                               "||fps:" +QString::number(fps));
}

/*
 * redefine the FRAMEINTERVAL to change the speed of refreash action
 */
void MainWindow::refresh_1s()
{
    static int cntTemp= 0;
    fps = matProThread.frameNum - cntTemp;
    cntTemp = matProThread.frameNum;
    if (matProThread.isRunning())
        ui->label_videoState->setStyleSheet("background-color: rgb(250, 0, 0)");
    else
        ui->label_videoState->setStyleSheet("background-color: rgb(0, 250, 0)");
}

/*
 */
void MainWindow::on_pushButton_snapshot_clicked()
{
    if(!matProThread.mat.empty())
    {
        imwrite("a.jpg",matProThread.mat);
    }
}

void MainWindow::on_pushButton_openVideo_clicked()
{
    //close first if open before
    if(matProThread.isRunning())
    {
        matProThread.stop();
        matProThread.wait();
    }

    //open
    //|-------------|
    //|1 camerel    |
    //|2 file       |
    //|-------------|
    QStringList items;
    items << tr("Cameral") << tr("File") ;
    bool ok;
    QString item = QInputDialog::getItem(this, tr("Select a input:"),
                                         tr("input:"), items, 0, false, &ok);
    if (ok && !item.isEmpty())
    {
        if("File"==item){
            //ask user the loc of file
            QString videoFileName = QFileDialog::getOpenFileName(this, tr("Open a video file"));
            if (!videoFileName.isEmpty())
            {
                matProThread.video = cv::VideoCapture(videoFileName.toStdString());
                if (matProThread.video.isOpened())
                {
                    ui->statusBar->showMessage("Open file "+videoFileName + " Success");
                    matProThread.set(maxBufSize);
                    matProThread.start();
                }
                else
                    ui->statusBar->showMessage(videoFileName + " is not a good VIDEO file",1000);
            }
        }
        //Open the Cameral with index 0
        if("Cameral"==item)
        {
            matProThread.video = cv::VideoCapture(0);
            if (matProThread.video.isOpened())
            {
                ui->statusBar->showMessage("Open file cameral 0");
                matProThread.set(maxBufSize);
                matProThread.start();
            }
            else
                ui->statusBar->showMessage("can't open cameral 0",1000);

        }
    }
}

void MainWindow::on_pushButton_pause_clicked()
{
    if(timer.isActive())
        timer.stop();
    else
        timer.start();
}

void MainWindow::on_radioButton_clicked()
{
    matProThread.setWaitUI(ui->radioButton->isChecked());
}
