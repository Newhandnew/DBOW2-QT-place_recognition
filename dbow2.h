#ifndef DBOW2_H
#define DBOW2_H

#include <iostream>

#include "opencv2/core.hpp"
#include "opencv2/xfeatures2d.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <DBoW2.h>

using namespace cv;
using namespace cv::xfeatures2d;
using namespace std;
using namespace DBoW2;

class DBow2 {
public:
    void setDatabase();
private:
    void loadVocabulary(String path);
    Surf64Vocabulary voc;
    Surf64Database vocDatabase;

};



#endif // DBOW2_H
