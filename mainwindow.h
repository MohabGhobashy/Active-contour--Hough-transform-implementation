#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "opencv2/world.hpp"

#include <QMainWindow>
#include <QLabel>
using namespace cv;
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    void showImg(Mat& img, QLabel* imgLbl, enum QImage::Format imgormat, int width, int hieght);

    ~MainWindow();
private slots:

    void on_actionUpload_triggered();
    void on_submitBtn_clicked();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
