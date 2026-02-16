#ifndef _USER_
#define _USER_
#include <string>
#include <vector>
using std::string;
using std::vector;
class Cave;
class Hike;

class User{
    string name;
    string passwd;
    vector<Cave> caveLogs;
    vector<Hike> hikeLogs;
    int id{};
    static int num_users;

public :
    // Getters
    string getName();
    string getPasswd();
    vector<Cave> &getCaveLogs();
    vector<Hike> &getHikeLogs();

    // Setters
    void setName(string name);
    void setPasswd(string passwd);
    void setCaveLogs(vector<Cave> caveLogs);
    void setHikeLogs(vector<Hike> hikeLogs);

    // General
    int addCaveLog();
    int addHikeLog();
    void removeCaveLog(int id);
    void removeHikeLog(int id);

    // Constructors
    User();
    User(string name);
    User(string name, string passwd);

};
#endif
