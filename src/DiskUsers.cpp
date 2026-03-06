#include "../include/DiskUsers.h"
#include <sstream>

DiskUsers::DiskUsers(): Disk(){}

void DiskUsers::parseLocations(){
    readFileContents();
    for (size_t i; i < fileContents.size(); ++i){
        if (fileContents[i].find("object: User") != std::string::npos)
        objectLocations.push_back(i);
    // i might be the goat
    }
}