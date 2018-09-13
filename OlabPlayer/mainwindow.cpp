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

#define SNAP_SHOT_PATH "C:/Users/Public/Pictures/"

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
 * every 1~100 ms
 */
void MainWindow::refresh()
{

    if(ui->radioButton->isChecked())
    {
        matProThread.mutex.lock();
        cv::Mat mat = matProThread.mat;
        matProThread.mutex.unlock();
        if (!mat.empty())
        {
            mainVideo = QtOcv::mat2Image(mat);
            ui->label_video->setPixmap(QPixmap::fromImage(mainVideo).scaled(ui->spinBox_cols->value(),ui->spinBox_rows->value()));
        }
        matProThread.getOneMat();
    }
    else
        ui->label_video->setText("Proccing Background...");

    ui->statusBar->showMessage("||Buf of frames:"+QString::number(matProThread.matBuf.size())+\
                               "||fps:" +QString::number(fps));
}

/*
 * redefine the FRAMEINTERVAL to change the speed of refreash action
 * fixed every 1 second
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
 * Save Img to SNAP_SHOT_PATH
 */
void MainWindow::on_pushButton_snapshot_clicked()
{
    if(!matProThread.mat.empty())
    {
        QString imName;
        imName = QDate::currentDate().toString("yyyy_MM_dd")+QTime::currentTime().toString("_HH_mm_ss")+".jpg";
        imName.insert(0,SNAP_SHOT_PATH);
        imwrite(imName.toStdString(),matProThread.mat);
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
    //|1 camere     |
    //|2 file       |
    //|-------------|
    QStringList items;
    items << tr("Camera") << tr("File") ;
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
                qDebug("Opened file: %s", qPrintable(videoFileName));
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
        //Open the Camera with index 0
        if("Camera"==item)
        {
            matProThread.video = cv::VideoCapture(0);
            if (matProThread.video.isOpened())
            {
                ui->statusBar->showMessage("Open file camera 0");
                matProThread.set(maxBufSize);
                matProThread.start();
            }
            else
                ui->statusBar->showMessage("can't open camera 0",1000);

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
