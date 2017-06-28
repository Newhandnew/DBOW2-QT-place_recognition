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
    connect(imageTimer, SIGNAL(timeout()), this, SLOT(getCamera()));
}

MainWindow::~MainWindow()
{
    cap.release();
    delete ui;
}

void MainWindow::openCamera(int deviceNum){
    cap.open(deviceNum);
    if(cap.isOpened() != 1)
    {
        cout << "error in capturing " << deviceNum << endl;
    }
    else {
        cout << "camera " << deviceNum << " start streaming..." << endl;
    }
}

QImage MainWindow::getQImage(cv::Mat &imgCV){

    static QVector<QRgb> colorTable;
    QImage imgReturn;
    if (colorTable.isEmpty()){
        for (int i = 0; i < 256; i++){
            colorTable.push_back(qRgb(i, i, i));
        }
    }

    if (imgCV.type() == CV_8UC3){
        QImage imgQImage = QImage((const unsigned char*)(imgCV.data), imgCV.cols, imgCV.rows, imgCV.step, QImage::Format_RGB888);
        imgReturn = imgQImage.rgbSwapped();
    }
    else if (imgCV.type() == CV_8UC1){
        QImage imgQImage = QImage((const unsigned char*)(imgCV.data), imgCV.cols, imgCV.rows, imgCV.step, QImage::Format_Indexed8);
        imgQImage.setColorTable(colorTable);
        imgReturn = imgQImage;
    }
    return imgReturn;
}

void MainWindow::getCamera() {
    cap >> frame;
    QImage imgQFrame = getQImage(frame);
    showVideo(imgQFrame);
}

void MainWindow::showVideo(QImage imgQFrame)
{
    ui->camera->setScaledContents(true);
    ui->camera->setPixmap(QPixmap::fromImage(imgQFrame));
    ui->camera->show();
}


void MainWindow::on_btn_startCamera_clicked()
{
    int deviceNumber;
    if(ui->radio_Camera1->isChecked()) {
        deviceNumber = 0;
    }
    else if(ui->radio_Camera2->isChecked()) {
        deviceNumber = 1;
    }

    openCamera(deviceNumber);
    imageTimer->start();
}

void MainWindow::on_btn_stopCamera_clicked()
{
    imageTimer->stop();
}
