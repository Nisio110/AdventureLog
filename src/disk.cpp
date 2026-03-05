#include "../include/disk.h"
#include <iostream>

using std::vector;
using std::string;
using std::ifstream;
using std::cout;


bool Disk::openDisk(){
    diskFile.open("disk.yaml");
    return !(!diskFile);
}
bool Disk::isFileAccessible(){
    return !(!diskFile);
}

vector<int> Disk::getFileInfo(){
    cout << "Disk accessible(?) : " << isFileAccessible() << "\n";
    return {1,2,3,4};
}

void IOTests::readFile(){
    string fileContents, ERR;
    Disk d;
    if (d.isFileAccessible()) cout << "Disk is accessible\n";
    else{
    while(std::getline(d,fileContents)){
            cout << fileContents << "\n";
        }
            cout << "EOF";
    }
}