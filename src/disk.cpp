#include "../include/disk.h"
#include <iostream>

using std::vector;
using std::string;
using std::ifstream;
using std::cout;

const string Disk::filename = "disk.yaml";
ifstream Disk::diskFile(filename);

Disk::Disk(){
    diskFile.open(filename);
}

ifstream& Disk::getDiskFile(){ return diskFile; }
void Disk::openDisk(){ diskFile.open(filename); }
bool Disk::isFileGood(){ return (diskFile.good()); }
const vector<int>& Disk::getFileInfo(){ return fileInfo; }

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