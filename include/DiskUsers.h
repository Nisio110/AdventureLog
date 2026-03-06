#include "Disk.h"

class DiskUsers : virtual public Disk{
    std::vector<User> users; 
public:
    DiskUsers();
    inline const std::vector<User>& getUsers(){return users;};

    // parsing
    void parseObjectNums();
    void parseLocations();
};