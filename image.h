#ifndef Image_H
#define Image_H

#include "opencv2/core/core.hpp"
#include <unordered_map>
#include <string>
#include <QPair>
using namespace std;
using namespace cv;
//#include "QTCV.h"

class Image1
{
private:
    Mat originalImg;
    Mat outputImage;
public:
    Image1();
    void setImage(Mat& img);
    Mat& getOutputImg();
    Mat& getOriginalImage();
    void reset();
};

#endif // Image_H



