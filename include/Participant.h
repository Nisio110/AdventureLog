#ifndef _PARTICIPANTS_
#define _PARTICIPANTS_
#include <vector>
#include <string>
using std::string;
using std::vector;

class Participant{
    string name;
    int id;
    static int numParticipants;
public :
    // Getters
    int getID();
    string getName();
    int getNumParticipants();

    // Setters
    void setID(int id);
    void setName(string names);
    void setNumParticipants(int num);

    // General
    int generateID();

    // Constructors
    Participant(string name);
};
#endif