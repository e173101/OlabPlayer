#include "matcooker.h"

MatCooker::MatCooker()
{

}

/* brief: the place you Test your algorithm here
 * @param mat: what you want to do for me？
 * @retrun mat: algorithm result
 */

Mat MatCooker::cook(Mat mat)
{
    if(!mat.empty())
    {
        Mat imageRGB[3];
        split(mat, imageRGB);
        return imageRGB[0];
    }
    return mat;
}
