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

	std::vector<User*> users;
	std::vector<Log*> logs;
	std::vector<Participant*> participants;

	void splitByObjects();
public:
	// Constructors
	Disk();
	~Disk();

	// Getters 
	inline std::string getFilePath(){return filePath;}
	inline const std::ifstream& getDiskFile(){return diskFile;}
	inline const std::vector<User*>& getUsers(){return users;}
	inline const std::vector<Log*>& getLogs(){return logs;}
	inline const std::vector<Participant*>& getParticipants(){return participants;}

	// Setters
	inline void setFilePath(std::string fp){filePath = fp;}

	inline void addUser(User* u){users.push_back(u);}
	inline void addLog(Log* log){logs.push_back(log);}
	inline void addParticipant(Participant* p){participants.push_back(p);}

	// File operations
	void openDisk();
	bool isFileGood();
	void readFileContents();

	// Parsing functions
	std::pair<std::string, std::string> parseStr(size_t lineNum); // returns key-value pair
	std::vector<std::pair<std::string, std::string>> parseFile();

	// Init functions
	void initProgram();
	User* initUser(std::vector<std::pair<std::string,std::string>> attr);
	Log* initLog(std::vector<std::pair<std::string,std::string>> attr);
	Participant* initParticipant(std::vector<std::pair<std::string,std::string>> attr);

	void printUserDetails(); // for testing
	void loadObject();
};

namespace IOTests{
	void readFile();
	void parseFile();
	void findUserObjectLocations();
}

#endif