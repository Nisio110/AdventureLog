#ifndef DISKUSER_H
#define DISKUSER_H
#include "Disk.h"

class DiskUsers : public Disk{
    std::vector<User> users; 
public:
    DiskUsers();
    inline const std::vector<User>& getUsers(){return users;};

    // parsing
    void parseObjectNums();
    void parseLocations();
    void parseObjectBodyRange();
};

#endif