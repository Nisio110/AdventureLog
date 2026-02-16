#ifndef _LOG_
#define _LOG_
#include <string>
#include <vector>
using std::string;
using std::vector;

class Log{
    int durationMins{}; // will add a setter that takes hours and minutes
    string location;
    vector<string> participants;
    vector<string> notes;
    string date;
    int id{};
public : 
    int getDurationMins();
    string getLocation();
    vector<string> getParticipants();
    vector<string> getNotes();
    string getDate();

    void setDurationMins(int duration);
    void setDuration(int hours, int mins);
    void setLocation(string location);
    void setParticipants(vector<string> participants);
    void setNotes(vector<string> notes);
    void setDate(string date);

    void addParticipant(string name);
    // search for name in the vector, might be better to make this based on id.
    void removeParticpant(string name); 
    //
};

class Cave : Log {
    bool srtCave{};
    bool caveLeader{};
    bool rigger{};
public : 
    bool isSrtCave();
    bool isCaveLeader();
    bool isRigger();

    bool setSrtCave(bool b);
    bool setCaveLeader(bool b);
    bool setRigger(bool b);

    void displayLog();
};

class Hike : Log {
    int distance{};
    string weather;
public : 
    int getDistance();
    string getWeather();
    void setDistance(int distance);
    void setWeather(string weather);

    void displayLog();
};
#endif
