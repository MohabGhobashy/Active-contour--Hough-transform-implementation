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
     showImg(image, ui->originalImg_2, QImage::Format_RGB888, ui->originalImg_2->width(), ui->originalImg_2->height());



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
    int points = 100;
    Size sz = original_image.mat.size();
    int x_cooridinates[points];
    int y_cooridinates[points];
    circle_contour(Point(sz.width / 2-20, sz.height / 2-20),70, points, x_cooridinates, y_cooridinates);
    Mat output=greedy_contour(original_image, 100, 1, 2,5, x_cooridinates, y_cooridinates, points,3, true);
//    int n = sizeof(x_cooridinates) / sizeof(int);

//    vector<int> code = chainCode(x_cooridinates, y_cooridinates, n);
//    std::vector<int> code = get_8_chain_code(x_cooridinates, y_cooridinates, 80);
    int dx[] = {0, 1, 1, 1, 0, -1, -1, -1};
    int dy[] = {-1, -1, 0, 1, 1, 1, 0, -1};
    int n = sizeof(x_cooridinates) / sizeof(int);
        int *code = new int[n-1];
        for (int i = 1; i < n; i++) {
            // Determine the direction of movement
            int dxn = x_cooridinates[i] - x_cooridinates[i-1];
            int dyn = y_cooridinates[i] - y_cooridinates[i-1];
            int direction = -1;
            for (int j = 0; j < 8; j++) {
                if (dx[j] == dxn && dy[j] == dyn) {
                    direction = j;
                    break;
                }
            }
            code[i-1] = direction;
        }
        for (int i = 0; i < n-1; i++) {
            qDebug() << "(" << dx[code[i]] << ", " << dy[code[i]] << ") ";
        }

    // Only consider the first contour (assumes there is only one contour in the image)

    showImg(output, ui->outputImg_2, QImage::Format_RGB888, ui->outputImg_2->width(), ui->outputImg_2->height());

}

