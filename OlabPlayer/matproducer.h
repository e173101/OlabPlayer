#ifndef MATPRODUCER_H
#define MATPRODUCER_H
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <QThread>
#include <QQueue>
#include <QMutex>

using namespace cv;

class MatProducer : public QThread
{
public:
    MatProducer();
    void set(VideoCapture *video, QQueue<Mat> *matBuf, int maxFrame);
    void stop(void);
    void run();
    QMutex mutex;


private:
    VideoCapture *video;
    QQueue<Mat> *matBuf;
    int maxFrame;
    volatile bool runFlag;
};

#endif // MATPRODUCER_H
