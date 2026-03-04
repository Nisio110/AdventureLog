#ifndef _IO_
#define _IO_
#include <vector>
#include "User.h"

class IO{
    int numObjects{}, numUsers{}, numLogs{}, numParticipants{};
    // vectors contain start and end lines for each object in the file.
    std::vector<int> userLocations{}, logLocations{}, participantLocations{}; 
    // Contains info about object numbers
    std::vector<int> fileInfo {numObjects, numUsers, numLogs, numParticipants};
    // TODO: sort each object type into its own section    
public:
    // Getters
    std::vector<int>* getFileInfo();

    // Setters
    // Decided against setters for now.

    // Constructors
    IO();

    // Read Operations
    bool isFileAccessible();
    std::vector<int>* readFileInfo(); // will write to fileInfo vector
    std::vector<User>* loadUsers(); 
    std::vector<Log>* loadLogs(); // will be called once the user has been selected. will load appropriate logs for the user
    std::vector<Participant>* loadParticipants(); // will be called inside loadLogs()

};

namespace IOTests{
    void readFile();
}
#endif