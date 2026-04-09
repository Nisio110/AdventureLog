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
    std::vector<CaveLog> caveLogs;
    std::vector<HikeLog> hikeLogs;
public :
    // Getters
    int getID();
    std::string getName();
    std::string getPasswd();
    std::vector<CaveLog>& getCaveLogs();
    std::vector<HikeLog>& getHikeLogs();

    // Setters
    void setID(int id);
    void setName(std::string name);
    void setPasswd(std::string passwd);
    void setCaveLogs(std::vector<CaveLog> &caveLogs);
    void setHikeLogs(std::vector<HikeLog> &hikeLogs);

    // General
    void print();
    void printAll();
    void generateID();
    void addCaveLog(CaveLog& caveLog);
    void addHikeLog(HikeLog& hikeLog);
    void removeCaveLog(int id);
    void removeHikeLog(int id);

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
