#include "../include/Disk.h"
#include <iostream>

using std::cout;

// Constructors
Disk::Disk(){
	openDisk();
	if (!isFileGood()) cout << "[ERROR] Disk is not accessible\n";
}

// Read operations
void Disk::openDisk(){ diskFile.open(filePath); }
bool Disk::isFileGood(){ return (diskFile.good()); }
void Disk::readFileContents(){
	std::string line;
	if (!isFileGood()) cout << "[ERROR] Disk is not accessible\n";
	else while( std::getline(diskFile >> std::ws, line))
		fileContents.push_back(line);
	// we want to be able to access each line of the file easily.
}

// Test Functions
/*
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
*/