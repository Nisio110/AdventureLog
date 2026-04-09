#ifndef PARTICIPANTS_H
#define PARTICIPANTS_H
#include <string>

class Participant{
    std::string name;
    int id;
    int logId{0};
    static int numParticipants;

    int generateID(); // private function
public :
    // Getters
    int getId();
    inline int getLogId() {return logId;}
    std::string getName();
    int getNumParticipants();

    // General
    void print();

    // Setters
    void setID(int id);
    inline void setLogID(int id) { logId = id;}
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