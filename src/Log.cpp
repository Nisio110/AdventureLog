#include "../include/Log.h"
#include <iostream>
#include <string>
#include "../include/Tests.h"
using std::string;
using std::cout;
using std::vector;
int Log::numLogs{0};
int CaveLog::numLogs{0};
int HikeLog::numLogs{0};

class User; // forward declaration

// LOG CLASS ========================================= //
// Getters
int Log::getDurationMins() { return durationMins; }
string Log::getArea() { return area; }
vector<Participant> Log::getParticipants() { return participants; }
string Log::getNote() { return note; }
string Log::getDate() { return date; }
int Log::getID(){ return id; }

// Setters
void Log::setDurationMins(int dMins){ durationMins = dMins; }
void Log::setDuration(int hours, int mins){ durationMins = (hours*60) + mins; }
void Log::setArea(string l){ area = l; }
void Log::setParticipants(vector<Participant> p){ participants = p; }
void Log::setNote(string nt){ note = nt; }
void Log::setDate(string d){ date = d; }
void Log::setID(int i){ id = i; }
void Log::setOwner(User *u){ owner = u; }

// General
void Log::display(){
    string small_div = "--- ";
    cout << small_div << "LOG" << small_div << '\n'
    << "Date: " << date << "\n"
    << "Area: " << area << "\n";
}
void Log::generateID(int &numObject){
// By dereferencing the pointer to numObject
// I am able to modify the static numObject value 
// stored in the class
    ++numLogs;
    setID(++numObject);
}
void Log::addParticipant(Participant &p){
    participants.push_back(p);
    p.setID(participants.size()-1);
}
void Log::removeParticipant(Participant &p){
   participants.erase(participants.begin() + p.getID());
} 
Log::Log(User* u, string date){
    generateID(numLogs);
    setOwner(u);
    setDate(date);
}
Log::Log(User* u, string date, string note){
    generateID();
    setOwner(u);
    setNote(note);
}
Log::Log(User* u, string date, string area, string note){
    generateID(numLogs);
    setOwner(u);
    setDate(date);
    setArea(area);
    setNote(note);
}
Log::Log(User* u, string date, string area, string note, vector<Participant> participants){
    generateID(numLogs);
    setOwner(u);
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

void CaveLog::display(){
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


    cout << div(3) << " CAVE LOG #" << getID() << " " << div(3) << '\n'
         << "Cave        : " << getName()         << "\n"
         << "Date        : " << getDate()         << "\n"
         << "SRT Cave    : " << _srtCave          << "\n"
         << "Area        : " << getArea()         << "\n"
         << "Cavers      : " << "TBC"             << "\n"
         << "Rigged      : " << _rigger           << "\n"
         << "Cave Leader : " << _caveLeader       << "\n"
         << div(4)                                << "\n"
         << "Notes : " << "\n" << getNote()       << "\n"
         << div(4)                                << "\n\n";
}

CaveLog::CaveLog(User *u, string name, string date): Log(u,date) {
    setName(name);
}
CaveLog::CaveLog(User *u, string name, string date, string area, string note): Log(u,date,area,note){
    setName(name);
} 
CaveLog::CaveLog(User *u, string name, string date, string area, string note, vector<Participant> participants, bool didSRT, bool isLeader, bool isRigger): Log(u,date,area,note,participants){
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

HikeLog::HikeLog(User* u, string date): Log(u,date){}
HikeLog::HikeLog(User* u, string date, string note): Log(u,date,note){}
HikeLog::HikeLog(User* u, string date, string note, int dist): Log(u, date, note){
    setDist(dist);
}
HikeLog::HikeLog(string date, string note, int dist, string weth, vector<Participant> &participants){
    generateID(numLogs);
    setDate(date);
    setNote(note);
    setDist(dist);
    setWeather(weth);
    setParticipants(participants);
}

// TESTING FUNCTIONS ================================================== //
void LogTests::testCaveLogConstructors(){
    using namespace Tests;
    CaveLog c1("Poll na Gollum", "12/01/2026");
    CaveLog c2("John Thomas","02/12/2025", "Fermanagh", "Went in crawled around, bit mucky");
    
    Participant p1("Martha Stewart"), p2("John Pork"), p3("Oran Blackwater");
    vector<Participant> vp {p1, p2, p3};
    CaveLog c3("Bruce's Pot", "24/11/2025", "Fermanagh", "Cool stretch of SRT over a river required to get to the entrance", vp, true, false, false);

    cout << div(2) << " TESTING CAVE LOG CONSTRUCTORS " << div(2) << "\n";
    c1.display();
    c2.display();
    c3.display();
    Tests::verifyTest();
}