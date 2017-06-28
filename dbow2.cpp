#include "dbow2.h"

void DBow2::loadVocabulary(String path) {
    String vocabulary_path("/home/new/Downloads/resources/surf64_k10L6.voc.gz");
    //    DBoW3::Vocabulary voc;
//    printf("loading vacabulary...");
    Surf64Vocabulary voc(vocabulary_path);
//    prinf("Done loading vacabulary");
}

