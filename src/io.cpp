#include "../include/io.h"
#include <fstream>
#include <iostream>
using std::vector;
using std::string;
using std::ifstream;
using std::cout;
using std::getline;


bool IO::isFileAccessible(){
    ifstream disk("disk.yaml");
    return !(!disk);
}

vector<int>* IO::getFileInfo(){
    cout << "Result of file open: " << isFileAccessible() << "\n";
}

void IOTests::readFile(){
    string fileContents, ERR;
    ERR = "[ERROR] ";
    ifstream disk("disk.yaml");
    if (!disk) cout << ERR << "Failed to open file\n";
    else {
        while(getline(disk,fileContents)){
            cout << fileContents << "\n";
        }
            cout << "EOF";
    }
}