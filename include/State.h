#ifndef STATE_H
#define STATE_H

#include "Disk.h"

class State{
    std::vector<User*> users;
    Disk disk;

public:
    inline std::vector<User*> getUsers(){return users;};
    inline void setUsers(std::vector<User*> users);

    void addUser(User* u);
    void removeUser(User* u);

    void save(Disk& d);
    void save();
};

#endif