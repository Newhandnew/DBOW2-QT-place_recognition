#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <opencv2/opencv.hpp>
#include <stdio.h>

using namespace cv;
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QImage getQImage(cv::Mat &imgCV);
    QTimer *imageTimer;
    Mat frame;
    VideoCapture cap;

public slots:
    void showVideo();

private slots:
    void on_btn_startCamera_clicked();
    void on_btn_stopCamera_clicked();
};

#endif // MAINWINDOW_H
