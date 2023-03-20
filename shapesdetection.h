#ifndef SHAPESDETECTION_H
#define SHAPESDETECTION_H
#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
using namespace std;
using namespace cv;

void circleDetection(Mat& img,int min_radius = 20, int max_radius = 120, int canny_threshold = 150, int accumulator_threshold = 150);



#endif // SHAPESDETECTION_H
