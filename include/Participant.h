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

    int generateID(); // private function
public :
    // Getters
    int getID();
    string getName();
    int getNumParticipants();

    // Setters
    void setID(int id);
    void setName(string names);
    void setNumParticipants(int num);

    // Constructors
    Participant(string name);
};

namespace ParticipantTests {
    void testConstructors();
}

#endif