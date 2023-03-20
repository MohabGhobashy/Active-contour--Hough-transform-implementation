#ifndef SHAPESDETECTION_H
#define SHAPESDETECTION_H
#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
using namespace std;
using namespace cv;

void circleDetection(Mat& img,int min_radius = 20, int max_radius = 500, int canny_threshold = 150, int accumulator_threshold = 120);
void setAccumulator(vector<vector<vector<int>>>& accumulator, Mat& img, vector<float>& cos_table , vector<float>& sin_table, int min_radius, int max_radius, int startY, int endY);
void drawCirles(vector<vector<vector<int>>>& accumulator, Mat& img, int min_radius, int max_radius, int startY, int endY, int accumulator_threshold);


#endif // SHAPESDETECTION_H
