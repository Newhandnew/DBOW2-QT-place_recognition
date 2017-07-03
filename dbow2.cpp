#include "dbow2.h"
#include <sys/stat.h>

DBow2::DBow2() {
    imageSaveNumber = 0;
    mkdir(imgSaveDir.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);   // create image folder
}

void DBow2::loadVocabulary(String path) {
    cout << "loading vacabulary..." << endl;
    voc.load(path);
    cout << "Done loading vacabulary" << endl;
}

void DBow2::setDatabase() {
    String vocabulary_path("/home/new/Downloads/resources/surf64_k10L6.voc.gz");
    loadVocabulary(vocabulary_path);
    cout << "set database..." << endl;
    vocDatabase.setVocabulary(voc);
    cout << "Done set database" << endl;
}

bool DBow2::checkKeyFrame(Mat frame, Mat &imgMatch) {
    double scoreThreshold = 0.1;
    bool result;
    QueryResults query;
    vector<vector<float>> currentDescriptors;
    extract_surf(frame, currentDescriptors);
    query = queryResult(currentDescriptors);
    if (query.size() == 0) {        // for firse time
        vocDatabase.add(currentDescriptors);
        saveImage(frame);
        result = false;
    }
    else {
        if (query[0].Score < scoreThreshold) {
            vocDatabase.add(currentDescriptors);
            saveImage(frame);
            result = false;
        }
        else {
            result = true;
            loadMachedImage(query[0].Id, imgMatch);
        }
    }
    return result;
}

void DBow2::extract_surf(const Mat & img, vector<vector<float>>& outDescriptors) {
    /* Extracts SURF interest points and their descriptors. */

    int minHessian = 400;
    Ptr<SURF> surf_detector = SURF::create(minHessian);
    vector<KeyPoint> keypoints;
    vector<float> descriptors;
    surf_detector->detectAndCompute(img, cv::Mat(), keypoints, descriptors);

    //change descriptor size
    const int L = surf_detector->descriptorSize();
    outDescriptors.resize(descriptors.size() / L);

    unsigned int j = 0;
    for (unsigned int i = 0; i < descriptors.size(); i += L, ++j) {
        outDescriptors[j].resize(L);
        std::copy(descriptors.begin() + i, descriptors.begin() + i + L, outDescriptors[j].begin());
    }
}

void DBow2::saveImage(Mat image) {
    stringstream imgName;
    imgName << imgSaveDir << imageSaveNumber << imgSaveType;
    imwrite(imgName.str(), image);
    imageSaveNumber++;
}

void DBow2::loadMachedImage(unsigned int name, Mat& imgMatch) {
    stringstream imgName;
    imgName << imgSaveDir << name << imgSaveType;
    imgMatch = imread(imgName.str(), CV_LOAD_IMAGE_COLOR);
}

QueryResults DBow2::queryResult(vector<vector<float>> &currentDescriptors) {
    QueryResults result;
    vocDatabase.query(currentDescriptors, result, 2);       // need to change because descriptor have to calculate
    cout << result << endl;
    return result;
}
