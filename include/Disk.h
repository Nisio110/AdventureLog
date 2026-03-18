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
	std::vector<std::pair<std::string, std::string>> attributes;
	std::vector<std::vector<std::pair<std::string,std::string>>> objects;
	std::vector<User> users;
	std::vector<Log> logs;
	std::vector<Participant> participants;

	void splitByObjects();
public:
	// Constructors
	Disk();

	// Getters 
	inline std::string getFilePath(){return filePath;}
	inline const std::ifstream& getDiskFile(){return diskFile;}

	// Setters
	inline void setFilePath(std::string fp){filePath = fp;}

	// File operations
	void openDisk();
	bool isFileGood();
	void readFileContents();

	// Parsing functions
	std::vector<size_t> parseStrLocs(std::string_view targetStr);
	std::vector<size_t> parseAttrLocs(std::string_view targetAttr);
	std::vector<size_t> parseAttrLocs(std::string_view targetAttr, std::string_view targetValue);
	std::string parseAttrValue(std::string_view targetAttr, std::vector<std::string_view> objectBody);
	size_t parseStrLoc(std::string_view targetStr, std::vector<std::string_view> searchTarget);
	std::vector<std::string> parseObjectBody(std::string targetObj, int objID);
	void parseObjectLocs();
	// The ones that matter
	std::pair<std::string, std::string> parseStr(size_t lineNum); // returns key-value pair
	std::vector<std::pair<std::string, std::string>> parseFile();
	// The ones that matter

	// Init functions
	void initProgram();
	User& initUser(std::vector<std::pair<std::string,std::string>> attr);
	Log& initLog(std::vector<std::pair<std::string,std::string>> attr);
	Participant& initParticipants(std::vector<std::pair<std::string,std::string>> attr);

	void loadObject();
};

namespace IOTests{
	void readFile();
	void parseFile();
	void findUserObjectLocations();
}

#endif