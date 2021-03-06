#ifndef MATPRODUCER_H
#define MATPRODUCER_H
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <QThread>
#include <QMutex>
#include "matcooker.h"

using namespace std;
using namespace cv;

class MatProducer : public QThread
{
public:
    MatProducer();
    void setWaitUI(bool wait);
    void getOneMat(void);
    void stop(void);
    void run();
    VideoCapture video;
    QMutex mutex;
    Mat mat;
    int frameNum;

private:
    volatile bool waitUIFlag;
    volatile bool runFlag;
    volatile bool UItakenFlag;
    Size matSize;
    MatCooker matcooker;
};

#endif // MATPRODUCER_H
