#ifndef STATE_H
#define STATE_H
#include "Disk.h"

class State{
    std::vector<User*> users;
    Disk disk;
    User* currentUser;
    static inline std::string defaultDiskPath {"disk.yaml"};

public:
    inline std::vector<User*> getUsers(){return users;};
    inline void setUsers(std::vector<User*> users);

    void addUser(User* u);
    void removeUser(User* u);
    void createUser(std::string username, std::string password);
    bool logIn(std::string username,std::string password);
    bool isUniqueUsername(std::string);
    inline User* getCurrentUser() {return currentUser;}
    inline void setCurrentUser(User* u){currentUser = u;}

    void loadSave(std::string path);
    void printAll();
    void save(Disk& d);
    void save();

    ~State();
    State(std::string path = defaultDiskPath);
};

#endif