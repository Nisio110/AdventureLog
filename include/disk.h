#ifndef DISK_H
#define DISK_H
#include <vector>
#include <fstream>
#include "User.h"
class Log;
class Participant;

class Disk{
    const std::string filename{"disk.yaml"};
    std::ifstream* diskFile;
    int numObjects{}, numUsers{}, numLogs{}, numParticipants{};
    // vectors contain start and end lines for each object in the file.
    std::vector<int> userLocations{}, logLocations{}, participantLocations{}; 
    // Contains info about object numbers
    std::vector<int> fileInfo {numObjects, numUsers, numLogs, numParticipants};
    // TODO: sort each object type into its own section    
public:
    // Getters
    std::vector<int> getFileInfo();
    std::ifstream* getDiskFile();

    // Setters
    // Decided against setters for now.

    // Constructors
    Disk();
    ~Disk();

    // Read Operations
    void openDisk();
    bool isFileGood();
    std::vector<int>* readFileInfo(); // will write to fileInfo vector
    std::vector<User>* loadUsers(); 
    std::vector<Log>* loadLogs(); // will be called once the user has been selected. will load appropriate logs for the user
    std::vector<Participant>* loadParticipants(); // will be called inside loadLogs()

};

namespace IOTests{
    void readFile();
    void parseFile();
}
#endif