#include "shapesdetection.h"
#include <thread>
#include <QDebug>
#include <cmath>


/////////////////////////////////////////////// Hough Line /////////////////////////////
cv::Mat houghLine(cv::Mat Orignalimage) {
    // Resize
    resize(Orignalimage, Orignalimage, cv::Size(500, round(500*Orignalimage.rows/Orignalimage.cols)));
    // Canny
    Mat Cannyimage;
    cvtColor(Orignalimage, Cannyimage, COLOR_BGR2GRAY);
    Canny(Cannyimage, Cannyimage, 130, 150);
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
//    cv::Mat output = Orignalimage.clone();
//    cv::cvtColor(output, output, cv::COLOR_GRAY2BGR);
    for (int i = 0; i < lines.size(); i++) {
        float rho = lines[i][0] - max_dist;
        float theta = angles[lines[i][1]];
        double a = cos(theta), b = sin(theta);
        double x0 = a * rho, y0 = b * rho;
        cv::Point pt1(cvRound(x0 + 1000 * (-b)), cvRound(y0 + 1000 * (a)));
        cv::Point pt2(cvRound(x0 - 1000 * (-b)), cvRound(y0 - 1000 * (a)));
        cv::line(Orignalimage, pt1, pt2, cv::Scalar(0, 0, 255), 1, cv::LINE_AA);
    }

    return Orignalimage;
}
/////////////////////////////////////// Hough Cirlce ////////////////////////////////////////////////////////////
void circleDetection(Mat& img,int min_radius, int max_radius , int canny_threshold , int accumulator_threshold){

    cv::resize(img, img, cv::Size(500, round(500*img.rows/img.cols)));
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
    int step = ceil(rows/20), startY = 0, endY = step;
    for(int i=0; i<20; i++){
        vecOfThreads.push_back(std::thread(setAccumulator, std::ref(accumulator), std::ref(edgy), std::ref(cos_table),
                                            std::ref(sin_table), min_radius, max_radius, startY, endY));
        startY += step;
        endY += step;
    }

    for (std::thread & th : vecOfThreads)
    {
        // If thread Object is Joinable then Join that thread.
        if (th.joinable())
            th.join();
    }
    vecOfThreads.clear();

     // Draw circles


    step = ceil(rows/10), startY = 0, endY = step;
    for(int i=0; i<10; i++){
        vecOfThreads.push_back(std::thread(drawCirles, std::ref(accumulator), std::ref(img), min_radius, max_radius, startY, endY, accumulator_threshold));
        startY += step;
        endY += step;
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
                if (accumulator[y][x][r - min_radius] > accumulator_threshold){
                    circle(img, Point(x, y), r, Scalar(0, 255, 0), 2);
                }
            }
        }
    }
}

/////////////////////////////////////// Hough Ellipse ////////////////////////////////////////////////////////////


Mat EllipseDetectionImplemented(Mat img){

    resize(img, img, cv::Size(256, 256));
    Mat output_image = img;

    // Apply Canny on image
    Mat Cannyimage;
    Canny(img, Cannyimage, 130, 150);

    // push edge pixels in array for optimization
    int rows = Cannyimage.rows;
    int cols = Cannyimage.cols;
    vector <Point> edge_points;
    for (int x = 0; x < rows; x++){
        for (int y = 0; y < cols; y++){
            if (Cannyimage.at<uchar>(x, y) > 0){
                edge_points.push_back(Point(x,y));
            }
        }
    }

    vector<std::thread> vecOfThreads;

    int step = floor(edge_points.size()/30);
    int startY = 0, endY = step;

//  use miltie cores of processor to speed up the itterations
    for(int i=0; i<30; i++){
        vecOfThreads.push_back(std::thread(setEllipseAcc, std::ref(output_image), std::ref(edge_points), startY, endY));
        startY += step;
        endY += step;
    }

    for (std::thread & th : vecOfThreads){
        // If thread Object is Joinable then Join that thread.
        if (th.joinable())
            th.join();
    }
    vecOfThreads.clear();

    return output_image;
}


void setEllipseAcc(Mat& output_image, vector<Point>& edge_points, int start,int end){

    // parameters
    float max_b_squared = round( 0.5 * 256);
    max_b_squared = max_b_squared * max_b_squared;
    float min_size = 4.0;
    int vote_threshold = 7;
    int p1, p2, p3, p1x, p1y, p2x, p2y, p3x, p3y;
    float xc, yc, a, b, d, k, dx, dy;
    float cos_tau_squared, b_squared, orientation;
    vector<float> acc;

    // take a pair of 2 points and assume they make the major axis of the ellipse
    for (p1=start; p1<end;p1++){
        p1x = edge_points[p1].x;
        p1y = edge_points[p1].y;

        for (p2=0; p2<p1; p2++){
            p2x = edge_points[p2].x;
            p2y = edge_points[p2].y;

            dx = p1x - p2x;
            dy = p1y - p2y;
            a = 0.5 * sqrt(dx * dx + dy * dy);

            if (a > 0.5 * min_size){
                xc = 0.5 * (p1x + p2x);
                yc = 0.5 * (p1y + p2y);

                // use every edge pixel to vote for the minor axis
                for (p3=0; p3<edge_points.size(); p3++){
                    p3x = edge_points[p3].x;
                    p3y = edge_points[p3].y;
                    dx = p3x - xc;
                    dy = p3y - yc;
                    d = sqrt(dx * dx + dy * dy);

                    if ( d > min_size && d <= a){
                        dx = p3x - p1x;
                        dy = p3y - p1y;
                        cos_tau_squared = ((a*a + d*d - dx*dx - dy*dy) / (2 * a * d));
                        cos_tau_squared = cos_tau_squared * cos_tau_squared;
                        k = a*a - d*d * cos_tau_squared;

                        if ( k>0 && cos_tau_squared<1){
                            b_squared = a*a * d*d * (1 - cos_tau_squared) / k;
                            if (b_squared <= max_b_squared){
                                acc.push_back(b_squared);
                            }
                        }
                    }
                }

                if (acc.size() > 0){

                    // get the most repeated minor axis value
                    for (int z = 0; z < acc.size(); z++){
                        acc.at(z) = round(acc.at(z));
                    }

                    int max_vote = 0;
                    float minor_axis;
                    for (int m=0 ; m<acc.size(); m++){
                        int counter = (int)count(acc.begin(), acc.end(), acc[m]);
                        if (counter > max_vote)
                        {
                            max_vote = counter;
                            minor_axis = acc.at(m);
                        }
                    }

                    // check if number of votes is above threshold
                    if (max_vote > vote_threshold){
                        orientation = atan2(p1x - p2x, p1y - p2y);
                        b = sqrt(minor_axis);

                        if (orientation != 0){
                            orientation = M_PI - orientation;
                            if (orientation > M_PI){
                                orientation = orientation - M_PI / 2.0 ;
                                a, b = b, a;
                            }
                        }
                        Point center(xc, yc);
                        Size axis(a, b);
                        cv::ellipse(output_image, center, axis, orientation, 0,360, cv::Scalar(255, 0, 0), 1);
                        acc.clear();
                    }
                }
            }
        }
    }
}

