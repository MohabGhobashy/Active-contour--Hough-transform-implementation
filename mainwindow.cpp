#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "opencv2/world.hpp"
#include <QFileDialog>
#include <QMessageBox>
#include <QPixmap>
#include <QImage>
#include <QVector>
#include <QQueue>
#include <QDebug>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include "image.h"
#include "shapesdetection.h"
#include "active_contour.hpp"
#include "image2.hpp"
#include "cvector.hpp"
#include "utils.hpp"
#include <iostream>
#include "opencv2/imgproc.hpp"
#include <cmath>



using namespace cv;
using namespace img;
using namespace std;

QString imgPath;
Image1 img1;


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->submitBtn->setDisabled(true);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::showImg(Mat& img, QLabel* imgLbl, enum QImage::Format imgFormat, int width , int hieght)
{
    QImage image((uchar*)img.data, img.cols, img.rows, imgFormat);
    QPixmap pix = QPixmap::fromImage(image);
    imgLbl->resize(img.rows, img.cols);
    imgLbl->setPixmap(pix.scaled(width, hieght, Qt::KeepAspectRatio));
//    imgLbl->setScaledContents(true);
}
void MainWindow::on_actionUpload_triggered()
{
    ui->submitBtn->setDisabled(false);
    imgPath = QFileDialog::getOpenFileName(this, "Open an Image", "..", "Images (*.png *.xpm *.jpg *.bmb)");
    qDebug()<<(imgPath);

    // read image using opencv
    if(imgPath.isEmpty())
        return;

    // tab 1

    Mat image = imread(imgPath.toStdString());
    cvtColor(image, image,COLOR_BGR2RGB);
    img1.setImage(image);
    cv::resize(image, image, cv::Size(500, round(500*image.rows/image.cols)));
    showImg(image, ui->originalImg, QImage::Format_RGB888, ui->originalImg->width(), ui->originalImg->height());
     ui->outputImg->clear();
    // tab 2
     showImg(image, ui->originalImg_2, QImage::Format_RGB888, ui->originalImg->width(), ui->originalImg->height());



}


void MainWindow::on_submitBtn_clicked()
{
    if(ui->typeComboBox->currentText() == "circle"){
        img1.reset();
        circleDetection(img1.getOutputImg());
        showImg(img1.getOutputImg(), ui->outputImg, QImage::Format_RGB888, ui->outputImg->width(), ui->outputImg->height());
    }
    else{
        string path = imgPath.toStdString();
        Mat image =imread(path, IMREAD_GRAYSCALE);
        Mat result = houghLine(image);
        showImg(result, ui->outputImg, QImage::Format_RGB888, ui->outputImg->width(), ui->outputImg->height());}
}


void MainWindow::on_submitBtn_2_clicked()
{
    string path = imgPath.toStdString();
    Image original_image = Image(path);
    int points = 80;
    Size sz = original_image.mat.size();
    int x_cooridinates[points];
    int y_cooridinates[points];
    circle_contour(Point(sz.width / 2, sz.height / 2-40),100, points, x_cooridinates, y_cooridinates);
    Mat output = greedy_contour(original_image, 50, 1, 2,5, x_cooridinates, y_cooridinates, points,3, true);
    showImg(output, ui->outputImg_2, QImage::Format_RGB888, ui->outputImg_2->width(), ui->outputImg_2->height());
}

