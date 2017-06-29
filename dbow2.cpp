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

void DBow2::checkKeyFrame(Mat frame) {
    vector<vector<float>> descriptors;
    extract_surf(frame, descriptors);
    vocDatabase.add(descriptors);
    saveImage(frame);
}

void DBow2::saveImage(Mat image) {
    stringstream imgName;
    imgName << imgSaveDir << imageSaveNumber << imgSaveType;
    imwrite(imgName.str(), image);
    imageSaveNumber++;
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
