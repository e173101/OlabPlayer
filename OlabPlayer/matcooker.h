#ifndef MATCOOKER_H
#define MATCOOKER_H

#include <opencv2/core.hpp>
using namespace cv;

class MatCooker
{
public:
    MatCooker();
    void cook(Mat mat);
};

#endif // MATCOOKER_H
