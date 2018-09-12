/* 视频生产者
 * 输入视频的路径，尽力把所有Mat传给matcooker
 *
 *
 */
#include "matproducer.h"


MatProducer::MatProducer()
{

}

void MatProducer::set(int maxBufSize)
{
    this->maxBufSize=maxBufSize;
    runFlag = true;
    setWaitUI(true);
}

void MatProducer::setWaitUI(bool wait)
{
    this->waitUIFlag = wait;
}

void MatProducer::run()
{
    //if you struct mat here and use cameral funning thing will hapen :)
    UItakenFlag = false;
    frameNum=0;
    while(runFlag)
    {
        //queue empty
        if(matBuf.size()<maxBufSize)
        {
            if(video.isOpened())
            {
                Mat mat;    //new mat every time
                video.read(mat);
                if(!mat.empty())
                {
                    matBuf.push_back(mat);  //enqueue
                }
            }
            else
                runFlag = false;
        }
        //queue full
        else
        {
            if(!matBuf.empty())
            {
                if(waitUIFlag)
                {
                    if(!UItakenFlag)    //haven taked
                    {
                        mutex.lock();
                        matBuf[0].copyTo(this->mat);
                        mat = matcooker.cook(mat);
                        mutex.unlock();
                        matBuf.erase(matBuf.begin());   //dequeue
                        UItakenFlag = true;
                        //cook ready UI can use the mat
                        frameNum++;
                    }
                    msleep(1);
                }
                else
                {
                    mat = matcooker.cook(mat);
                    frameNum++;
                    matBuf.erase(matBuf.begin()); //dequeue
                }
            }
        }
    }
}

//tell the thread I'd read the mat
void MatProducer::getOneMat(void)
{
    if(UItakenFlag)
    {
        UItakenFlag = false;
    }
}

void MatProducer::stop()
{
    runFlag = false;
}
