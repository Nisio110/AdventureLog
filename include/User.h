#ifndef USER_H
#define USER_H

#include <string>
#include <vector>
#include "Log.h"

class User{
    int id{};
    static int numUsers;
    std::string name{"N/A"};
    std::string passwd;
    std::vector<Log*> logs;
public :
    // Getters
    int getID();
    std::string getName();
    std::string getPasswd();
    std::vector<Log*> getLogs();

    // Setters
    void setID(int id);
    void setName(std::string name);
    void setPasswd(std::string passwd);
    void setLogs(std::vector<Log*> logs);

    // General
    void print();
    void printAll();
    void generateID();
    void addLog(Log* log);
    void removeLog(int id);

    // Constructors
    User();
    User(std::string name);
    User(std::string name, std::string passwd);
};

namespace UserTests {
    void testCaveLogsIO(); // add and remove CaveLog objects
    void testHikeLogsIO(); // add and remove HikeLog objects
    void testGenerateID();
    void testConstructors();
}
#endif
