#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <time.h>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    database = new DBow2(); //initial DBow2
    // camera setting
    int cameraFPS = 25;
    imageTimer = new QTimer(this);
    imageTimer->setInterval(getPeriod(cameraFPS));
    connect(imageTimer, SIGNAL(timeout()), this, SLOT(getCamera()));
    // save image setting
    int checkImageFPS = 2;
    saveTimer = new QTimer(this);
    saveTimer->setInterval(getPeriod(checkImageFPS));
    connect(saveTimer, SIGNAL(timeout()), this, SLOT(checkImage()));
}

MainWindow::~MainWindow()
{
    cap.release();
    delete ui;
}

int MainWindow::getPeriod(int fps) {
    return 1000 / fps;      //ms
}

void MainWindow::openCamera(int deviceNum) {
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

void MainWindow::checkImage() {
    Mat imgMatching;
    bool match;
    unsigned int matchId;
    clock_t start = clock();
    match = database->checkKeyFrame(frame, imgMatching, matchId);
    if(match) {
        QImage imgQFrame = getQImage(frame);
        showSaveImage(imgQFrame);

        QImage imgQMatch = getQImage(imgMatching);
        showRecognition(imgQMatch);
        std::string s = std::to_string(matchId);
//        char const *pchar = s.c_str();  //use char const* as target type
        ui->label_recognitionId->setText(s.c_str());
    }
    else {
        QImage imgQFrame;
        showSaveImage(imgQFrame);
        showRecognition(imgQFrame);
        ui->label_recognitionId->setText("No recognition");
    }
    clock_t end = clock();
    float spendSeconds = (float)(end - start) / CLOCKS_PER_SEC;
    cout<< "time: " << spendSeconds << endl;
}

void MainWindow::recognition() {
    QImage imgQFrame = getQImage(frame);
    showRecognition(imgQFrame);
}

void MainWindow::showVideo(QImage imgQFrame)
{
    ui->camera->setScaledContents(true);
    ui->camera->setPixmap(QPixmap::fromImage(imgQFrame));
    ui->camera->show();
}

void MainWindow::showSaveImage(QImage imgQFrame)
{
    ui->img_temp->setScaledContents(true);
    ui->img_temp->setPixmap(QPixmap::fromImage(imgQFrame));
    ui->img_temp->show();
}

void MainWindow::showRecognition(QImage imgQFrame)
{
    ui->img_recognition->setScaledContents(true);
    ui->img_recognition->setPixmap(QPixmap::fromImage(imgQFrame));
    ui->img_recognition->show();
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
    saveTimer->start();
//    recognitionTimer->start();
}

void MainWindow::on_btn_stopCamera_clicked()
{
    imageTimer->stop();
    saveTimer->stop();
//    recognitionTimer->stop();
}

void MainWindow::on_btn_loadDatabase_clicked()
{
    database->setDatabase();
}
