#include "image.h"
using namespace std;
using namespace cv;

Image1::Image1(){}

void Image1::setImage(Mat& img)
{
    this->originalImg = img.clone();
    this->outputImage = img.clone();

}

void Image1::reset(){
    this->outputImage = this->originalImg.clone();

}

Mat& Image1::getOutputImg(){
    return this->outputImage;
}

Mat& Image1::getOriginalImage(){
    return this->originalImg;
}



