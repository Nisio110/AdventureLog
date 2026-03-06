#include "../include/DiskUsers.h"
#include <sstream>
#include <iostream>

DiskUsers::DiskUsers(): Disk(){}

void DiskUsers::parseLocations(){
    readFileContents();
    for (size_t i{}; i < fileContents.size(); ++i){
        if (fileContents[i].find("object: User") != std::string::npos)
        objectLocations.push_back(i);
    // i might be the goat
    }
}
void IOTests::findUserObjectLocations(){
	DiskUsers du;
	du.parseLocations();
	for (const auto& lineNum : du.getObjectLocations())
		std::cout << "User object found at line: " << lineNum << std::endl;
}