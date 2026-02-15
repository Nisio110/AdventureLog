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
    int numHikeLogs{};
    int numCaveLogs{};
    int numLogs{};
    
    vector<Cave> caveLogs;
    vector<Hike> hikeLogs;
public :
    string getName();
    string getPassword();
    int getNumLogs();
    int getNumHikeLogs();
    int getNumCaveLogs();
    Cave getCaveLog(int id);
    Hike getHikeLog(int id);
    void setName(string name);
    void setPasswd(string passwd);
    
    int addCaveLog();
    int addHikeLog();
    void removeCaveLog(int id);
    void removeHikeLog(int id);
};
#endif
