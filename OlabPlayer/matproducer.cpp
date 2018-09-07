/* 视频生产者
 *
 *
 */
#include "matproducer.h"



MatProducer::MatProducer()
{

}

void MatProducer::set(VideoCapture *video, QQueue<Mat> *matBuf, int maxFrame, bool resizeFlat, int resizeCols, int resizeRows)
{
    this->video=video;
    this->matBuf=matBuf;
    this->maxFrame=maxFrame;
    this->resizeFlag=resizeFlat;
    this->resizeCols=resizeCols;
    this->resizeRows=resizeRows;
    runFlag = true;
}

void MatProducer::run()
{
    //Mat mat; //if you struct mat here and use cameral funning thing will hapen :)
    while(runFlag)
    {
        if(matBuf->size()<maxFrame)
        {
            if(video->isOpened())
            {
                Mat mat;    //new mat every time
                mutex.lock();
                video->read(mat);
                if(!mat.empty())
                {
                    if(resizeFlag)
                        cv::resize(mat,mat,Size(resizeCols,resizeRows));
                    matBuf->enqueue(mat);
                }
                mutex.unlock();
            }
            else
                runFlag = false;
        }
        msleep(1);
    }
}

void MatProducer::stop()
{
    runFlag = false;
}
