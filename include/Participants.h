#ifndef _PARTICIPANTS_
#define _PARTICIPANTS_
#include <vector>
#include <string>
using std::string;
using std::vector;

class Participants{
    int id{-1};
    vector<string> names;
public :
    // Getters
    int getID();
    vector<string> getNames();

    // Setters
    void setID(int id);
    void setNames(vector<string> names);

    // General
    void addName(string name);
    void removeName(int pid);
};

#endif