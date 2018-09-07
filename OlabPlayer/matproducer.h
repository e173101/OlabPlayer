#ifndef MATPRODUCER_H
#define MATPRODUCER_H
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <QThread>
#include <QQueue>
#include <QMutex>

using namespace cv;

class MatProducer : public QThread
{
public:
    MatProducer();
    void set(VideoCapture *video, QQueue<Mat> *matBuf, int maxFrame, bool resizeFlag = true, int resizeCols = 640, int resizeRows = 320);
    void stop(void);
    void run();
    QMutex mutex;


private:
    VideoCapture *video;
    QQueue<Mat> *matBuf;
    int maxFrame;
    volatile bool runFlag;
    bool resizeFlag;
    int resizeCols;
    int resizeRows;
};

#endif // MATPRODUCER_H
