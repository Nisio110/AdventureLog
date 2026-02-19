#ifndef _LOG_
#define _LOG_
#include "Participant.h"
#include <string>
#include <vector>
using std::string;
using std::vector;

class Log{
protected:
    int durationMins{}; // will add a setter that takes hours and minutes
    string area{"N/A"};
    vector<Participant> participants;
    string note;
    string date{"N/A"};
    int id{-1};        // initialised to -1 to make errors obvious
public :
    // Getters
    int getDurationMins();
    string getArea();
    string getNote();
    string getDate();
    vector<Participant> getParticipants();
    int getID();

    // Setters
    void setDurationMins(int duration);
    void setDuration(int hours, int mins);
    void setArea(string location);
    void setNote(string notes);
    void setDate(string date);
    void setID(int id);
    void setParticipants(vector<Participant> p);

    // General
    void display();
    void addParticipant(Participant &p);
    void removeParticipant(Participant &p);
};

class CaveLog : public Log {
    string name;
    bool srtCave{};
    bool caveLeader{};
    bool rigger{};
public : 
    // Getters
    string getName();
    bool wasSRTCave();
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
    CaveLog();
    CaveLog(string name, string date);
    CaveLog(string name, string date, string location, string note);
    CaveLog(string name, string date, string location, string note, vector<Participant> participants, bool srt, bool leader, bool rigger);
};

class HikeLog : public Log {
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
    HikeLog();
    HikeLog(string date);
    HikeLog(string date, string note);
    HikeLog(string date, string note, int distance);
    HikeLog(string date, string note, int distance, string weather, vector<Participant> participants);
};

// Log Class Tests
namespace LogTest{
    void testCaveLogConstructors();
    void testHikeLogConstructors();
    void testParticipantsIO(); // add and remove participants
    void testDisplay();
}

#endif