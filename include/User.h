#ifndef _USER_
#define _USER_
#include <string>
#include <vector>
using std::string;
using std::vector;
#include "Log.h"

class User{
    int id{};
    static int numUsers;
    string name{"N/A"};
    string passwd;
    vector<CaveLog> caveLogs;
    vector<HikeLog> hikeLogs;

public :
    // Getters
    int getID();
    string getName();
    string getPasswd();
    vector<CaveLog> &getCaveLogs();
    vector<HikeLog> &getHikeLogs();

    // Setters
    void setID(int id);
    void setName(string name);
    void setPasswd(string passwd);
    void setCaveLogs(vector<CaveLog> &caveLogs);
    void setHikeLogs(vector<HikeLog> &hikeLogs);

    // General
    void generateID();
    void addCaveLog(CaveLog& clog);
    void addHikeLog(HikeLog& hlog);
    void removeCaveLog(int id);
    void removeHikeLog(int id);

    // Constructors
    User();
    User(string name);
    User(string name, string passwd);
};

namespace UserTests {
    void testCaveLogsIO(); // add and remove CaveLog objects
    void testHikeLogsIO(); // add and remove HikeLog objects
    void testGenerateID();
    void testConstructors();
}

#endif
