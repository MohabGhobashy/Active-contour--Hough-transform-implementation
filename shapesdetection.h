#ifndef SHAPESDETECTION_H
#define SHAPESDETECTION_H
#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
using namespace std;
using namespace cv;

// HoughLine Algo
struct AccumulatorResult {
    cv::Mat accumulator;
    int max_dist;
    std::vector<double> angles;
};

float deg2rad(int deg);
AccumulatorResult accumulator(cv::Mat& Cannyimage);
std::vector<cv::Vec2i> findPeaks(cv::Mat accumulator);
cv::Mat drawLineOnImg(cv::Mat image , std::vector<cv::Vec2i> lines , std::vector<double> angles,int max_dist);
cv::Mat houghLine(cv::Mat originalImg);

// HoughCirlce Algo
void circleDetection(Mat& img,int min_radius = 20, int max_radius = 250, int canny_threshold = 150, int accumulator_threshold = 150);
void setAccumulator(vector<vector<vector<int>>>& accumulator, Mat& img, vector<float>& cos_table , vector<float>& sin_table, int min_radius, int max_radius, int startY, int endY);
void drawCirles(vector<vector<vector<int>>>& accumulator, Mat& img, int min_radius, int max_radius, int startY, int endY, int accumulator_threshold);

// HoughEllipse Algo
Mat EllipseDetectionImplemented(Mat img);
void setEllipseAcc(Mat& output_image, vector<Point>& edge_points, int start,int end);

#endif // SHAPESDETECTION_H
