#include "shapesdetection.h"



void circleDetection(Mat& img,int min_radius, int max_radius , int canny_threshold , int accumulator_threshold){

    cv::resize(img, img, cv::Size(500, 500));
    Mat edgy;
    cvtColor(img, edgy, COLOR_BGR2GRAY);
    Canny(edgy, edgy, canny_threshold, canny_threshold * 2);
    int num_angles = 360;
    float angle_step = 2*M_PI / num_angles;
    float cos_table[num_angles];
    float sin_table[num_angles];
    for (int i = 0; i < num_angles; i++) {
        cos_table[i] = cos(i * angle_step);
        sin_table[i] = sin(i * angle_step);
    }
    int rows = edgy.rows;
    int cols = edgy.cols;
    vector<vector<vector<int>>> accumulator(rows, vector<vector<int>>(cols, vector<int>(max_radius - min_radius + 1)));


    #pragma omp parallel for
    for (int y = 0; y < rows; y++) {
        for (int x = 0; x < cols; x++) {
            if (edgy.at<uchar>(y, x) > 0) {
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

     // Draw circles
    for (int y = 0; y < rows; y++) {
        for (int x = 0; x < cols; x++) {
            for (int r = min_radius; r <= max_radius; r++) {
                if (accumulator[y][x][r - min_radius] > accumulator_threshold) {
                    circle(img, Point(x, y), r, Scalar(0, 255, 0), 2);
                }
            }
        }
    }
}
