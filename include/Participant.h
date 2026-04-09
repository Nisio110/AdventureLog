#ifndef PARTICIPANTS_H
#define PARTICIPANTS_H
#include <string>

class Participant{
    std::string name;
    int id;
    int logID{0};
    static int numParticipants;

    int generateID(); // private function
public :
    // Getters
    int getID();
    inline int getLogID() {return logID;}
    std::string getName();
    int getNumParticipants();

    // General
    void print();

    // Setters
    void setID(int id);
    inline void setLogID(int id) { logID = id;}
    void setName(std::string names);
    void setNumParticipants(int num);

    // Constructors
    Participant(std::string _name);
    Participant(std::string _name, int _logID);
    Participant(std::string _name, int _logID, int _id);
};

namespace ParticipantTests {
    bool testConstructors();
}

#endif