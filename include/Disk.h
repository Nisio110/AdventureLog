#ifndef DISK_H
#define DISK_H
#include <vector>
#include <fstream>
#include "User.h"
class Log;
class Participant;

class Disk{
	// File variables
	std::string filePath;
	std::ifstream diskFile;

	// Vectors for storing objects 
	std::vector<User> users;
	std::vector<CaveLog> caveLogs;
	std::vector<HikeLog> hikeLogs;
	std::vector<Participant> participants;

	// Contains object count stats
	int numObjects{};
	int numUsers{};
	int numCaveLogs{};
	int numHikeLogs{};
	int numParticipants{};

	// the location vectors will contain start and end line nums for each object in the file.
	std::vector<int> userLocations;
	std::vector<int> logLocations;
	std::vector<int> participantLocations; 

public:
	// Constructors
	Disk();

	// Getters 
	inline std::string getFilePath(){return filePath;}
	inline const std::ifstream& getDiskFile(){return diskFile;}
	inline int getNumObjects(){return numObjects;}
	inline int getNumUsers(){return numUsers;}
	inline int getNumCaveLogs(){return numCaveLogs;}
	inline int getNumHikeLogs(){return numHikeLogs;}
	inline int getNumParticipants(){return numParticipants;}

	// Setters
	inline void setFilePath(std::string fp){filePath = fp;}


	// File operations
	void openDisk();
	bool isFileGood();

	// Parsing functions
	void parseFileInfo(); // will write to numObject variables
	void parseUserLocations();
	void parseCaveLogLocations();
	void parseHikeLogLocations();
	void parseParticipantLocations();

	// Object creation functions
	void loadUsers(); 
	void loadCaveLogs();
	void loadHikeLogs();
	void loadParticipants();
};

namespace IOTests{
	void readFile();
	void parseFile();
}

#endif