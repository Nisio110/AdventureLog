#include "../include/disk.h"
#include <iostream>

using std::vector;
using std::string;
using std::ifstream;
using std::cout;

Disk::Disk(){
    diskFile = new ifstream;
    diskFile->open(filename);
}
Disk::~Disk(){
    delete diskFile;
}

ifstream* Disk::getDiskFile(){ return diskFile; }
void Disk::openDisk(){ diskFile->open(filename); }
bool Disk::isFileGood(){ return (diskFile->good()); }

vector<int> Disk::getFileInfo(){
    return fileInfo;
}

void IOTests::readFile(){
    string fileContents, ERR;
    Disk d;
    if (!d.isFileGood()) cout << "Disk is not accessible\n";
    else{
        while(std::getline(d.getDiskFile(),fileContents)){
            cout << fileContents << "\n";
        }
        cout << "EOF";
    }
}