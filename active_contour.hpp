#ifndef __SNAKE__
#define __SNAKE__

#include <opencv2/opencv.hpp>
#include <cmath>
#include "cvector.hpp"
#include "image2.hpp"
#include <stdio.h>
#include "utils.hpp"
double area_triangle(double a, double b, double c);
void circle_contour(cv::Point center, double radius, int numberOfPoints, int *x_cooridinates, int *y_cooridinates);
void draw_contour(cv::Mat Image, int numberOfPoints, int *x_cooridinates, int *y_cooridinates);
double contour_area(int numberOfPoints, int *x_cooridinates, int *y_cooridinates);
double contour_perimeter(int *x_points, int *y_points, int points_n);
cvector<cvector<int>> window_neighbours(int size);
cv::Mat greedy_contour(img::Image source, int iterations ,float alpha, float beta, float gamma,int* x_points, int* y_points, int points_n, int window_size, bool plot);
#endif
