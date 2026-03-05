#ifndef PARTICIPANTS_H
#define PARTICIPANTS_H
#include <string>

class Participant{
    std::string name;
    int id;
    static int numParticipants;

    int generateID(); // private function
public :
    // Getters
    int getID();
    std::string getName();
    int getNumParticipants();

    // Setters
    void setID(int id);
    void setName(std::string names);
    void setNumParticipants(int num);

    // Constructors
    Participant(std::string name);
};

namespace ParticipantTests {
    bool testConstructors();
}

#endif