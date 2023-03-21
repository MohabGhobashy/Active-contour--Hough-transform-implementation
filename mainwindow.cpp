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
using namespace cv;
QString imgPath;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionUpload_triggered()
{
    imgPath = QFileDialog::getOpenFileName(this, "Open an Image", "..", "Images (*.png *.xpm *.jpg *.bmb)");

    // read image using opencv
    if(imgPath.isEmpty())
        return;

    // tab 1

    Mat image = imread(imgPath.toStdString());
    cvtColor(image, image,COLOR_BGR2RGB);
}
void MainWindow::showImg(Mat& img, QLabel* imgLbl, enum QImage::Format imgFormat, int width , int hieght)
{
    QImage image2((uchar*)img.data, img.cols, img.rows, imgFormat);
    QPixmap pix = QPixmap::fromImage(image2);
}
