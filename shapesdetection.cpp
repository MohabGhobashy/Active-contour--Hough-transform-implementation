#include "shapesdetection.h"
#include <thread>

/////////////////////////////////////////////// Hough Line /////////////////////////////
cv::Mat houghLine(cv::Mat Orignalimage) {
    // Resize
    resize(Orignalimage, Orignalimage, cv::Size(500, 500));
    // Canny
    Mat Cannyimage;
    Canny(Orignalimage, Cannyimage, 130, 150);
    // Define range of angles to scan
    std::vector<double> angles;
    for (int i = -90; i <= 90; i++) {
        angles.push_back(i * CV_PI / 180.0);
    }

    // Create accumulator array
    int max_dist = cvRound(sqrt(pow(Cannyimage.rows, 2) + pow(Cannyimage.cols, 2)));
    cv::Mat accumulator = cv::Mat::zeros(max_dist * 2 + 1, angles.size(), CV_32SC1);

    // Loop through image pixels
    for (int y = 0; y < Cannyimage.rows; y++) {
        for (int x = 0; x < Cannyimage.cols; x++) {
            if (Cannyimage.at<uchar>(y, x) > 0) {
                // Calculate rho for each angle
                for (int a = 0; a < angles.size(); a++) {
                    int rho = cvRound(x * cos(angles[a]) + y * sin(angles[a])) + max_dist;
                    accumulator.at<int>(rho, a)++;
                }
            }
        }
    }

    // Find local maxima in accumulator
    std::vector<cv::Vec2i> lines;
    int threshold = 170;
    for (int r = 0; r < accumulator.rows; r++) {
        for (int a = 0; a < accumulator.cols; a++) {
            if (accumulator.at<int>(r, a) > threshold) {
                lines.push_back(cv::Vec2i(r, a));
            }
        }
    }

    // Draw lines on image
    cv::Mat output = Orignalimage.clone();
    cv::cvtColor(output, output, cv::COLOR_GRAY2BGR);
    for (int i = 0; i < lines.size(); i++) {
        float rho = lines[i][0] - max_dist;
        float theta = angles[lines[i][1]];
        double a = cos(theta), b = sin(theta);
        double x0 = a * rho, y0 = b * rho;
        cv::Point pt1(cvRound(x0 + 1000 * (-b)), cvRound(y0 + 1000 * (a)));
        cv::Point pt2(cvRound(x0 - 1000 * (-b)), cvRound(y0 - 1000 * (a)));
        cv::line(output, pt1, pt2, cv::Scalar(0, 0, 255), 1, cv::LINE_AA);
    }

    return output;
}
/////////////////////////////////////// Hough Cirlce ////////////////////////////////////////////////////////////
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
