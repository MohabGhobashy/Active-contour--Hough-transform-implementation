#include "shapesdetection.h"
#include <thread>



void circleDetection(Mat& img,int min_radius, int max_radius , int canny_threshold , int accumulator_threshold){

    cv::resize(img, img, cv::Size(500, 500));
    Mat edgy;
    cvtColor(img, edgy, COLOR_BGR2GRAY);
    Canny(edgy, edgy, canny_threshold, canny_threshold * 2);
    int num_angles = 360;
    float angle_step = 2*M_PI / num_angles;
    vector<float> cos_table;
    vector<float> sin_table;
//    float cos_table[num_angles];
//    float sin_table[num_angles];
    for (int i = 0; i < num_angles; i++) {
        cos_table.push_back(cos(i * angle_step));
        sin_table.push_back(sin(i * angle_step));
    }
    int rows = edgy.rows;
    int cols = edgy.cols;
    vector<vector<vector<int>>> accumulator(rows, vector<vector<int>>(cols, vector<int>(max_radius - min_radius + 1)));
    vector<std::thread> vecOfThreads;
    int startY = 0, endY = 25;
    for(int i=0; i<20; i++){
        vecOfThreads.push_back(std::thread(setAccumulator, std::ref(accumulator), std::ref(edgy), std::ref(cos_table),
                                            std::ref(sin_table), min_radius, max_radius, startY, endY));
        startY += 25;
        endY += 25;
    }

    for (std::thread & th : vecOfThreads)
    {
        // If thread Object is Joinable then Join that thread.
        if (th.joinable())
            th.join();
    }
    vecOfThreads.clear();

     // Draw circles


    startY = 0, endY = 50;
    for(int i=0; i<10; i++){
        vecOfThreads.push_back(std::thread(drawCirles, std::ref(accumulator), std::ref(img), min_radius, max_radius, startY, endY, accumulator_threshold));
        startY += 50;
        endY += 50;
    }
    for (std::thread & th : vecOfThreads)
    {
        // If thread Object is Joinable then Join that thread.
        if (th.joinable())
            th.join();
    }
    vecOfThreads.clear();
}

void setAccumulator(vector<vector<vector<int>>>& accumulator, Mat& img, vector<float>& cos_table,
                    vector<float>& sin_table, int min_radius, int max_radius, int startY, int endY){
    int rows = img.rows;
    int cols = img.cols;
    for (int y = startY; y < endY; y++) {
        for (int x = 0; x < cols; x++) {
            if (img.at<uchar>(y, x) > 0) {
                for (int r = min_radius; r <= max_radius; r++) {
                    for (int theta = 0; theta < 360; theta++) {
                        int a = x - r * cos_table[theta];
                        int b = y - r * sin_table[theta];
                        if (a >= 0 && a < cols && b >= 0 && b < rows) {
                            accumulator[b][a][r - min_radius]++;
                        }
                    }
                }
            }
        }
    }
}


void drawCirles(vector<vector<vector<int>>>& accumulator, Mat& img, int min_radius, int max_radius, int startY, int endY, int accumulator_threshold){

    int cols = img.cols;
    for (int y = startY; y < endY; y++) {
        for (int x = 0; x < cols; x++) {
            for (int r = min_radius; r <= max_radius; r++) {
                if (accumulator[y][x][r - min_radius] > accumulator_threshold) {
                    circle(img, Point(x, y), r, Scalar(0, 255, 0), 2);
                }
            }
        }
    }
}
