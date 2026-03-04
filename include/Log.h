#ifndef _LOG_H
#define _LOG_H
#include "Participant.h"
#include <string>
#include <vector>
class User;

class Log{
protected:
    int id{-1};        // initialised to -1 to make errors obvious
    int durationMins{}; // will add a setter that takes hours and minutes
    std::string area{"N/A"};
    std::vector<Participant> participants;
    std::string note;
    std::string date{"N/A"};
    static int numLogs;
    User* owner;
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
    void setParticipants(std::vector<Participant> p);
    void setOwner(User* u);

    // General
    void display();
    void generateID(int &numObject);
    void addParticipant(Participant &p);
    void removeParticipant(Participant &p);

    // Constructors
    Log(User* u, std::string date);
    Log(User* u, std::string date, std::string note);
    Log(User* u, std::string date, std::string area, std::string note);
    Log(User* u, std::string date, std::string area, std::string note, std::vector<Participant> participants);

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
    CaveLog(User* u, std::string name, std::string date);
    CaveLog(User* u, std::string name, std::string date, std::string area, std::string note);
    CaveLog(User* u, std::string name, std::string date, std::string area, std::string note, std::vector<Participant> participants, bool isSRT, bool wasCL, bool wasRigg);
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
    HikeLog(User* u, std::string date);
    HikeLog(User* u, std::string date, std::string note);
    HikeLog(User* u, std::string date, std::string area, std::string note, int distance);
    HikeLog(User* u, std::string date, std::string area, std::string note, int distance, std::string weather, std::vector<Participant> &participants);
};

// Log Class Tests
namespace LogTests{
    void testCaveLogConstructors(User* u);
    void testHikeLogConstructors(User* u);
    void testParticipantsIO(); // add and remove participants
    void testDisplay();
}

#endif