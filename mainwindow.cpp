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
using namespace cv;


QString imgPath;
Image img;
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
    imgLbl->setPixmap(pix.scaled(width, hieght,Qt::KeepAspectRatio));
}

void MainWindow::on_actionUpload_triggered()
{
    ui->submitBtn->setDisabled(false);
    imgPath = QFileDialog::getOpenFileName(this, "Open an Image", "..", "Images (*.png *.xpm *.jpg *.bmb)");
    // read image using opencv
    if(imgPath.isEmpty())
        return;

    // tab 1

    Mat image = imread(imgPath.toStdString());
    cvtColor(image, image,COLOR_BGR2RGB);
    img.setImage(image);
    cv::resize(image, image, cv::Size(500, 500));
    showImg(image, ui->originalImg, QImage::Format_RGB888, ui->originalImg->width(), ui->originalImg->height());
     ui->outputImg->clear();


}


void MainWindow::on_submitBtn_clicked()
{
    if(ui->typeComboBox->currentText() == "circle"){
        circleDetection(img.getOutputImg());
        showImg(img.getOutputImg(), ui->outputImg, QImage::Format_RGB888, ui->outputImg->width(), ui->outputImg->height());
    }
}

