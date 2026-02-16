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

public :
    // Getters
    string getName();
    string getPassword();
    Cave getCaveLog(int id);
    Hike getHikeLog(int id);

    // Setters
    void setName(string name);
    void setPasswd(string passwd);

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
