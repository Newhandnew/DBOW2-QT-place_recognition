#include "mainwindow.h"
#include "ui_mainwindow.h"
//VideoCapture cap(0);
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    const int imagePeriod = 1000 / 40;   // ms
    imageTimer = new QTimer(this);
    imageTimer->setInterval(imagePeriod);
    connect(imageTimer, SIGNAL(timeout()), this, SLOT(showVideo()));
    cap.open(0);
    if(cap.isOpened() != 1)
    {
        printf("error in capturing\n");
    }
    printf("start streaming...\n");

}

MainWindow::~MainWindow()
{
    cap.release();
    delete ui;
}

QImage MainWindow::getQImage(cv::Mat &imgCV){

    static QVector<QRgb> colorTable;

    if (colorTable.isEmpty()){
        for (int i = 0; i < 256; i++){
            colorTable.push_back(qRgb(i, i, i));
        }
    }

    if (imgCV.type() == CV_8UC3){
        QImage imgQImage = QImage((const unsigned char*)(imgCV.data), imgCV.cols, imgCV.rows, imgCV.step, QImage::Format_RGB888);
        return imgQImage.rgbSwapped();
    }
    else if (imgCV.type() == CV_8UC1){
        QImage imgQImage = QImage((const unsigned char*)(imgCV.data), imgCV.cols, imgCV.rows, imgCV.step, QImage::Format_Indexed8);
        imgQImage.setColorTable(colorTable);
        return imgQImage;
    }
}

void MainWindow::showVideo()
{
    cap >> frame;
    QImage imgQFrame = getQImage(frame);
    ui->camera->setScaledContents(true);
    ui->camera->setPixmap(QPixmap::fromImage(imgQFrame));
    ui->camera->show();
}


void MainWindow::on_btn_startCamera_clicked()
{
    imageTimer->start();
}

void MainWindow::on_btn_stopCamera_clicked()
{
    imageTimer->stop();
}
