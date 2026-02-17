#ifndef _LOG_
#define _LOG_
#include <string>
#include <vector>
using std::string;
using std::vector;

class Log{
protected:
    int durationMins{}; // will add a setter that takes hours and minutes
    string area{"N/A"};
    vector<string> participants;
    vector<string> notes;
    string date{"N/A"};
    int id{-1};                 // initialised to -1 to make errors obvious
    int participantID{-1}; 
public : 
    int getDurationMins();
    string getLocation();
    vector<string> getParticipants();
    vector<string> getNotes();
    string getDate();
    int getID();
    int getPID();

    void setDurationMins(int duration);
    void setDuration(int hours, int mins);
    void setLocation(string location);
    void setParticipants(vector<string> participants);
    void setNotes(vector<string> notes);
    void setDate(string date);
    void setID(int id);

    void addParticipant(string name);
    // search for name in the vector, might be better to make this based on id.
    void removeParticpant(int pID); 
    //
    void display();
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
    CaveLog(string name, string date, string location, vector<string> notes);
    CaveLog(string name, string date, string location, vector<string> notes, vector<string> participants, bool srt, bool leader, bool rigger);
};

class HikeLog : public Log {
    int distance{};
    string weather;
public : 
    // Getters
    int getDistance();
    string getWeather();
    // Setters
    void setDistance(int distance);
    void setWeather(string weather);

    // General
    void display();

    // Constructors
    HikeLog();
    HikeLog(int distance);
    HikeLog(int distance, string weather);
};
#endif
