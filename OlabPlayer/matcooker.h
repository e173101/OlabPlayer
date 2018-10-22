#ifndef MATCOOKER_H
#define MATCOOKER_H

#include <opencv2/core.hpp>
#include <opencv2/video.hpp>
using namespace cv;

class MatCooker
{
public:
    MatCooker();
    void cook(Mat mat);
private:

    RNG rng;
};

#endif // MATCOOKER_H
