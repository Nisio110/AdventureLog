#include "../include/io.h"
#include <fstream>
#include <iostream>
using std::vector;

vector<int>* getFileInfo(){
    vector<int>* fi = new vector<int>;
    *fi = {0,0,0,0};
    std::string fileContents, ERR;
    ERR = "[ERROR] ";
    std::ifstream disk("disk.yaml");
    if (!disk) std::cout << ERR << "Failed to open file\n";
    else{
        while(std::getline(disk,fileContents))
            std::cout << fileContents << "\n";
            std::cout << "EOF";
    }
    return fi;
}

void IOTests::readFile(){
    vector<int> fi { *getFileInfo() };
}