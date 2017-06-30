#ifndef DBOW2_H
#define DBOW2_H

#include <iostream>

#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"  // for imwrite
#include "opencv2/xfeatures2d.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <DBoW2.h>

using namespace cv;
using namespace cv::xfeatures2d;
using namespace std;
using namespace DBoW2;

class DBow2 {
public:
    DBow2();
    void setDatabase();
    bool checkKeyFrame(Mat frame, Mat& imgMatch);

private:
    void loadVocabulary(String path);
    void saveImage(Mat image);
    void extract_surf(const Mat & img, vector<vector<float>>& outDescriptors);
    QueryResults queryResult(vector<vector<float>>& currentDescriptors);

    Surf64Vocabulary voc;
    Surf64Database vocDatabase;
    const string imgSaveDir = "imgEnv/";
    const string imgSaveType = ".jpg";
    int imageSaveNumber;

};



#endif // DBOW2_H
