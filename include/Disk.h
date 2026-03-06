#ifndef DISK_H
#define DISK_H
#include <vector>
#include <fstream>
#include "User.h"
class Log;
class Participant;

class Disk{
protected:
	std::string filePath;
	std::ifstream diskFile;
	
	std::vector<std::string> fileContents;
	int numObjects{};
	// contains start and end line numbers for each object
	std::vector<size_t> objectLocations;
public:
	// Constructors
	Disk();

	// Getters 
	inline std::string getFilePath(){return filePath;}
	inline const std::ifstream& getDiskFile(){return diskFile;}
	inline int getNumObjects(){return numObjects;}
	inline std::vector<size_t> getObjectLocations(){return objectLocations;}

	// Setters
	inline void setFilePath(std::string fp){filePath = fp;}

	// File operations
	void openDisk();
	bool isFileGood();
	void readFileContents();

	// Parsing functions
	//virtual void parseObjectNums() = 0; // will write to numObject variables
	virtual void parseLocations() = 0;

	void loadObject();
};

namespace IOTests{
	void readFile();
	void parseFile();
	void findUserObjectLocations();
}

#endif