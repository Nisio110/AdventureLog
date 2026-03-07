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
	// line numbers for every object definition
	std::vector<size_t> objectLocs;
	// Start and end line numbers of an object definitions body
	const std::string objectType;
public:
	// Constructors
	Disk();

	// Getters 
	inline std::string getFilePath(){return filePath;}
	inline const std::ifstream& getDiskFile(){return diskFile;}
	inline int getNumObjects(){return numObjects;}
	inline std::vector<size_t> getObjectLocs(){return objectLocs;}

	// Setters
	inline void setFilePath(std::string fp){filePath = fp;}
	inline void setNumObjects(int num){numObjects = num;}

	// File operations
	void openDisk();
	bool isFileGood();
	void readFileContents();

	// Parsing functions
	std::vector<size_t> parseStrLocs(std::string_view targetStr);
	std::vector<size_t> parseAttrLocs(std::string_view targetAttr);
	std::vector<size_t> parseAttrLocs(std::string_view targetAttr, std::string_view targetValue);
	std::string parseAttrValue(std::string_view targetAttr, std::vector<std::string> objectBody);
	size_t parseStrLoc(std::string_view targetStr, std::vector<std::string_view> searchTarget);
	std::pair<std::string, std::string> parseStr(size_t lineNum); // returns key-value pair
	std::vector<std::string> parseObjectBody(std::string targetObj, int objID);
	std::vector<std::pair<std::string, std::string>> parseFile();
	void parseObjectLocs();

	void loadObject();
};

namespace IOTests{
	void readFile();
	void parseFile();
	void findUserObjectLocations();
}

#endif