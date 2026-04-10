#include "../include/Log.h"
#include <iostream>
#include <string>
#include "../include/Tests.h"
using std::string;
using std::vector;
using std::cout;
using std::vector;
int Log::numLogs{0};

// LOG CLASS ========================================= //
// Getters
int Log::getDurationMins() { return durationMins; }
string Log::getArea() { return area; }
vector<Participant*> Log::getParticipants() { return participants; }
string Log::getNote() { return note; }
string Log::getDate() { return date; }
int Log::getId(){ return id; }
int Log::getUserId(){ return userId; }

// Setters
void Log::setDurationMins(int dMins){ durationMins = dMins; }
void Log::setDuration(int hours, int mins){ durationMins = (hours*60) + mins; }
void Log::setArea(string l){ area = l; }
void Log::setParticipants(vector<Participant*> p){ participants = p; }
void Log::setNote(string nt){ note = nt; }
void Log::setDate(string d){ date = d; }
void Log::setID(int i){ id = i; }
void Log::setUserId(int uid){ userId = uid; }

// General
void Log::print(){
    string small_div = "--- ";
    cout << small_div << "LOG" << small_div << small_div << small_div << '\n'
    << "Date: " << date << "\n"
    << "Area: " << area << "\n"
    << "Participants: " << small_div;
    for (auto p : getParticipants()){
        p->print();
    }
    cout << small_div << small_div << small_div << small_div << small_div << '\n';
}
void Log::generateID(int &numObject){
// By dereferencing the pointer to numObject
// I am able to modify the static numObject value
// stored in the class
    ++numLogs;
    setID(++numObject);
}
void Log::seedIdCounter(int n){ numLogs = n; }
void Log::addParticipant(Participant* p){
    participants.push_back(p);
}
void Log::removeParticipant(Participant* p){
   participants.erase(participants.begin() + p->getId());
} 
Log::Log(){}
Log::Log(int uid, string date){
    setUserId(uid);
    generateID(numLogs);
    setDate(date);
}
Log::Log(int uid, string date, string note){
    generateID(numLogs);
    setUserId(uid);
    setNote(note);
}
Log::Log(int uid, string date, string area, string note){
    generateID(numLogs);
    setUserId(uid);
    setDate(date);
    setArea(area);
    setNote(note);
}
Log::Log(int uid, string date, string area, string note, vector<Participant*> participants){
    generateID(numLogs);
    setUserId(uid);
    setDate(date);
    setArea(area);
    setNote(note);
}

// CAVELOG CLASS ========================================= //
string CaveLog::getName(){ return name; }
bool CaveLog::isSRTCave(){ return isSRT; }
bool CaveLog::wasCaveLeader(){ return wasCL; }
bool CaveLog::wasRigger(){ return didRigging; }
void CaveLog::setName(string n){ name = n; }
void CaveLog::setSRTCave(bool b){ isSRT = b; }
void CaveLog::setCaveLeader(bool b){ wasCL = b; }
void CaveLog::setRigger(bool b){ didRigging = b; }

void CaveLog::print(){
    using namespace Tests;
    string pos{"Yes"},neg{"No"};
    string _rigger;
    string _srtCave;
    string _caveLeader;
    if (wasRigger()) _rigger = pos;
    else _rigger = neg;
    if (wasCaveLeader()) _caveLeader = pos;
    else _caveLeader = neg;
    if (isSRTCave()) _srtCave = pos;
    else _srtCave = neg;


    cout << div(1) << " Log #" << getId() << " - Cave " << div(2) << '\n'
         << "Cave        : " << getName()         << "\n"
         << "Date        : " << getDate()         << "\n"
         << "SRT Cave    : " << _srtCave          << "\n"
         << "Area        : " << getArea()         << "\n"
         << "Cavers      : " << "TBC"             << "\n"
         << "Rigged      : " << _rigger           << "\n"
         << "Cave Leader : " << _caveLeader       << "\n"
         << "Participants: " << "\n";

    for (auto p : getParticipants())
        {
            cout << "  ";
            p->print();
        }

    cout << div(3)                                << "\n"
         << "Notes : " << "\n" << getNote()       << "\n"
         << div(3)                                << "\n\n";
}

CaveLog::CaveLog(){}
CaveLog::CaveLog(int uid, string name, string date): Log(uid,date) {
    setName(name);
}
CaveLog::CaveLog(int uid, string name, string date, string area, string note): Log(uid,date,area,note){
    setName(name);
} 
CaveLog::CaveLog(int uid, string name, string date, string area, string note, vector<Participant*> participants, bool didSRT, bool isLeader, bool isRigger): Log(uid,date,area,note,participants){
    setName(name);
    setSRTCave(didSRT);
    setCaveLeader(isLeader);
    setRigger(isRigger);
}

// HIKELOG CLASS ========================================= //
int HikeLog::getDist(){ return distance; }
string HikeLog::getWeather(){ return weather; }

void HikeLog::setDist(int d){ distance = d; }
void HikeLog::setWeather(string w){ weather = w; }

void HikeLog::print(){
    using namespace Tests;
    cout << div(1) << " Log #" << getId() << " - Hike " << div(2) << '\n'
         << "Date        : " << getDate()         << "\n"
         << "Area        : " << getArea()         << "\n"
         << "Distance    : " << getDist()         << "\n"
         << "Weather     : " << getWeather()      << "\n"
         << "Duration    : " << getDurationMins() << " mins" << "\n"
         << "Participants: " << "\n";

    for (auto p : getParticipants())
        { p->print(); }

    cout << div(3)                                << "\n"
         << "Notes : " << "\n" << getNote()       << "\n"
         << div(3)                                << "\n\n";
}

HikeLog::HikeLog(){}
HikeLog::HikeLog(int uid, string date): Log(uid,date){}
HikeLog::HikeLog(int uid, string date, string note): Log(uid,date,note){}
HikeLog::HikeLog(int uid, string date, string area, string note, int dist): Log(uid, date, area, note){
    setDist(dist);
}
HikeLog::HikeLog(int uid, string date, string area, string note, int dist, string weth, vector<Participant*> &participants): Log(uid,date,area, note,participants){
    setDist(dist);
    setWeather(weth);
}

// TESTING FUNCTIONS ================================================== //
void LogTests::testCaveLogConstructors(int uid){
    using namespace Tests;
    CaveLog c1(uid, "Poll na Gollum", "12/01/2026");
    CaveLog c2(uid, "John Thomas","02/12/2025", "Fermanagh", "Went in crawled around, bit mucky");
    
    Participant p1("Martha Stewart"), p2("John Pork"), p3("Oran Blackwater");
    vector<Participant*> vp {&p1, &p2, &p3};
    CaveLog c3(uid, "Bruce's Pot", "24/11/2025", "Fermanagh", "Cool stretch of SRT over a river required to get to the entrance", vp, true, false, false);

    cout << div(2) << " TESTING CAVE LOG CONSTRUCTORS " << div(2) << "\n";
    c1.print();
    c2.print();
    c3.print();
    Tests::verifyTest();
}