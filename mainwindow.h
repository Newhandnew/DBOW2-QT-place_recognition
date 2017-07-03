#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <opencv2/opencv.hpp>
#include <stdio.h>
#include <iostream>

#include "dbow2.h"

using namespace cv;
using namespace std;

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
    int getPeriod(int fps);
    QImage getQImage(cv::Mat &imgCV);
    void openCamera(int deviceNum);
    void showVideo(QImage imgQFrame);
    void showSaveImage(QImage imgQFrame);
    void showRecognition(QImage imgQFrame);

    QTimer *imageTimer;
    QTimer *saveTimer;
    Mat frame;
    VideoCapture cap;
    DBow2 *database;

public slots:
    void getCamera();
    void checkImage();
    void recognition();

private slots:
    void on_btn_startCamera_clicked();
    void on_btn_stopCamera_clicked();
    void on_btn_loadDatabase_clicked();
};

#endif // MAINWINDOW_H
