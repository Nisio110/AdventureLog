#ifndef _LOG_H
#define _LOG_H
#include "Participant.h"
#include <string>
#include <vector>

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
    std::string getArea();
    std::string getNote();
    std::string getDate();
    std::vector<Participant> getParticipants();
    int getID();
    User* getOwner();

    // Setters
    void setDurationMins(int duration);
    void setDuration(int hours, int mins);
    void setArea(std::string area);
    void setNote(std::string notes);
    void setDate(std::string date);
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

class CaveLog : public Log {
    static int numLogs;
    std::string name{"N/A"};
    bool isSRT{};
    bool wasCL{};
    bool didRigging{};
public : 
    // Getters
    std::string getName();
    bool isSRTCave();
    bool wasCaveLeader();
    bool wasRigger();
    // Setters
    void setName(std::string name);
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

class HikeLog : public Log {
    static int numLogs;
    int distance{-1};
    std::string weather{"N/A"};
public : 
    // Getters
    int getDist();
    std::string getWeather();
    // Setters
    void setDist(int distance);
    void setWeather(std::string weather);

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