#include "dbow2.h"

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
