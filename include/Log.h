#ifndef _LOG_
#define _LOG_
#include "Participant.h"
#include <string>
#include <vector>
using std::string;
using std::vector;

class Log{
protected:
    int id{-1};        // initialised to -1 to make errors obvious
    int durationMins{}; // will add a setter that takes hours and minutes
    static int numLogs;
    string date{"N/A"};
    string area{"N/A"};
    string note;
    User* owner;
    vector<Participant> participants;
public :
    // Getters
    int getDurationMins();
    string getArea();
    string getNote();
    string getDate();
    vector<Participant> getParticipants();
    int getID();
    User* getOwner();

    // Setters
    void setDurationMins(int duration);
    void setDuration(int hours, int mins);
    void setArea(string area);
    void setNote(string notes);
    void setDate(string date);
    void setID(int id);
    void setParticipants(vector<Participant> p);
    void setOwner(User *u);

    // General
    void display();
    void generateID(int &numObject);
    void addParticipant(Participant &p);
    void removeParticipant(Participant &p);

    // Constructors
    Log(User* u, string date);
    Log(User* u, string date, string note);
    Log(User* u, string date, string area, string note);
    Log(User* u, string date, string area, string note, vector<Participant> participants);

};

class CaveLog : virtual public Log {
    static int numLogs;
    string name{"N/A"};
    bool isSRT{};
    bool wasCL{};
    bool didRigging{};
public : 
    // Getters
    string getName();
    bool isSRTCave();
    bool wasCaveLeader();
    bool wasRigger();
    // Setters
    void setName(string name);
    void setSRTCave(bool b);
    void setCaveLeader(bool b);
    void setRigger(bool b);

    // General
    void display();

    // Constructors
    CaveLog(User* u, string name, string date);
    CaveLog(User* u, string name, string date, string area, string note);
    CaveLog(User* u, string name, string date, string area, string note, vector<Participant> participants, bool isSRT, bool wasCL, bool wasRigg);
};

class HikeLog : virtual public Log {
    static int numLogs;
    int distance{-1};
    string weather{"N/A"};
public : 
    // Getters
    int getDist();
    string getWeather();
    // Setters
    void setDist(int distance);
    void setWeather(string weather);

    // General
    void display();

    // Constructors
    HikeLog(User* u, string date);
    HikeLog(User* u, string date, string note);
    HikeLog(User* u, string date, string note, int distance);
    HikeLog(User* u, string date, string note, int distance, string weather, vector<Participant> &participants);
};

// Log Class Tests
namespace LogTests{
    void testCaveLogConstructors();
    void testHikeLogConstructors();
    void testParticipantsIO(); // add and remove participants
    void testDisplay();
}

#endif