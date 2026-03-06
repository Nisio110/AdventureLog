#include "../include/Disk.h"
#include <iostream>

using std::vector;
using std::string;
using std::ifstream;
using std::cout;

// Constructors
Disk::Disk(){
    diskFile.open(filePath);
}

// Getters
ifstream& Disk::getDiskFile(){ return diskFile; }


// Read operations
void Disk::openDisk(){ diskFile.open(filePath); }
bool Disk::isFileGood(){ return (diskFile.good()); }


// Test Functions
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