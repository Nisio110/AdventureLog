#include "../include/Disk.h"
#include <iostream>

using std::cout;

// Constructors
Disk::Disk(){
	setFilePath("disk.yaml");
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

std::vector<size_t> Disk::parseStrLocs(std::string_view targetStr){
	std::vector<size_t> lineNums;
	readFileContents();
	for (size_t i{}; i < fileContents.size(); ++i){
		if (fileContents[i].starts_with(targetStr) && fileContents[i].find(targetStr) != std::string::npos)
			lineNums.push_back(i);
	}
	return lineNums;
}

std::vector<size_t> Disk::parseAttrLocs(std::string_view targetAttr){
	// Goal: Discern the location that an attribute can be found on.
	std::string targetStr;
	std::string assign{": "};
	targetStr.append(targetAttr).append(assign);

	return parseStrLocs(targetStr);
}

std::vector<size_t> Disk::parseAttrLocs(std::string_view targetAttr, std::string_view targetValue){
	std::string targetStr;
	std::string assign{": "};
	targetStr.append(targetAttr).append(assign).append(targetValue);

	return parseStrLocs(targetStr);
}

size_t parseStrLoc(std::string_view targetStr, std::vector<std::string_view> searchTarget){
	// In contrast to parseStrLocs, this function assumes only one instance of the target string
	// is present in the search target, and hence only returns one size_t location variable.
	size_t lineNum;
	for (size_t i{}; i < searchTarget.size(); ++i){
		if (searchTarget[i].starts_with(targetStr) && searchTarget[i].find(targetStr) != std::string::npos)
			lineNum = i;
	}
	return lineNum;
}

std::string parseAttrValue(std::string_view targetAttr, std::vector<std::string_view> objectBody){
	std::string assign{": "};
	std::string _targetAttr;
	_targetAttr.append(targetAttr).append(assign);
	
	size_t attrLoc {parseStrLoc(_targetAttr, objectBody)};
	return _targetAttr.substr(_targetAttr.size(), objectBody[attrLoc].size() - _targetAttr.size());


}

std::vector<std::string> parseObjectBody(std::string targetObj, int objID){
	std::string attr {"object"};
	std::vector<size_t> objLocs {parseAttrLocs(attr, targetObj)};
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