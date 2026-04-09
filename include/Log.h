#ifndef LOG_H
#define LOG_H
#include "Participant.h"
#include <string>
#include <vector>
class User;

class Log{
protected:
    int id{-1};        // initialised to -1 to make errors obvious
    int durationMins{}; // will add a setter that takes hours and minutes
    std::string area{"N/A"};
    std::vector<Participant*> participants;
    std::string note;
    std::string date{"N/A"};
    static int numLogs;
    int userId{};
public :
    // Getters
    int getDurationMins();
    std::string getArea();
    std::string getNote();
    std::string getDate();
    std::vector<Participant*> getParticipants();
    int getId();
    int getUserId();

    // Setters
    void setDurationMins(int duration);
    void setDuration(int hours, int mins);
    void setArea(std::string area);
    void setNote(std::string notes);
    void setDate(std::string date);
    void setID(int _id);
    void setParticipants(std::vector<Participant*> p);
    void setUserId(int _uid);

    // General
    virtual void print();
    void generateID(int &numObject);
    void addParticipant(Participant* p);
    void removeParticipant(Participant* p);

    // Constructors & Destructor
    Log();
    virtual ~Log() = default;
    Log(int uid, std::string date);
    Log(int uid, std::string date, std::string note);
    Log(int uid, std::string date, std::string area, std::string note);
    Log(int uid, std::string date, std::string area, std::string note, std::vector<Participant*> participants);

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
    void print() override;

    // Constructors
	CaveLog();
    CaveLog(int uid, std::string name, std::string date);
    CaveLog(int uid, std::string name, std::string date, std::string area, std::string note);
    CaveLog(int uid, std::string name, std::string date, std::string area, std::string note, std::vector<Participant*> participants, bool isSRT, bool wasCL, bool wasRigg);
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
    void print() override;

    // Constructors
	HikeLog();
    HikeLog(int uid, std::string date);
    HikeLog(int uid, std::string date, std::string note);
    HikeLog(int uid, std::string date, std::string area, std::string note, int distance);
    HikeLog(int uid, std::string date, std::string area, std::string note, int distance, std::string weather, std::vector<Participant*> &participants);
};

// Log Class Tests
namespace LogTests{
    void testCaveLogConstructors(int uid);
    void testHikeLogConstructors(int uid);
    void testParticipantsIO(); // add and remove participants
    void testDisplay();
}

#endif