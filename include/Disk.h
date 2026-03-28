#ifndef DISK_H
#define DISK_H
#include <vector>
#include <fstream>
#include "User.h"
class Log;
class Participant;

using KeyValue     = std::pair<std::string, std::string>;
using KeyValueList = std::vector<KeyValue>;
using ObjectList   = std::vector<KeyValueList>;

class Disk{
protected:
	std::string filePath;
	std::ifstream diskFile;
	std::vector<std::string> fileContents;
	KeyValueList attributes;
	ObjectList objects;

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
	inline std::ifstream& getDiskFile(){return diskFile;}
	inline std::vector<User*>& getUsers(){return users;}
	inline std::vector<Log*>& getLogs(){return logs;}
	inline std::vector<Participant*>& getParticipants(){return participants;}

	// Setters
	inline void setFilePath(std::string fp){filePath = fp;}

	inline void addUser(User* u){users.push_back(u);}
	inline void addLog(Log* log){logs.push_back(log);}
	inline void addParticipant(Participant* p){participants.push_back(p);}

	// File operations
	void openDisk(std::string_view fp);
	bool isDiskGood();
	void readDiskContents(std::string_view fp);

	// Parsing functions
	KeyValue parseStr(size_t lineNum); // returns key-value pair
	KeyValueList parseFile();

	// Init functions
	void initProgram();
	User* initUser(KeyValueList attr);
	Log* initLog(KeyValueList attr);
	Participant* initParticipant(KeyValueList attr);

	void printUserDetails(); // for testing
	void loadObject();
};

namespace IOTests{
	void readFile();
	void parseFile();
	void findUserObjectLocations();
}

#endif