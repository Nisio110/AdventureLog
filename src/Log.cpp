#include "../include/Log.h"
#include <iostream>
#include <string>
using std::string;
using std::cout;

// LOG CLASS ========================================= //
// Getters
int Log::getDurationMins() {return durationMins;}
string Log::getArea() {return area;}
vector<string> Log::getParticipants() {return participants;}
string Log::getNote() {return note;}
string Log::getDate() {return date;}
int Log::getID(){ return id; }

// Setters
void Log::setDurationMins(int dMins){ durationMins = dMins; }
void Log::setDuration(int hours, int mins){ durationMins = (hours*60) + mins; }
void Log::setArea(string l){ area = l; }
void Log::setParticipants(vector<string> ps){ participants = ps; }
void Log::setNote(string nt){ note = nt; }
void Log::setDate(string d){ date = d; }
void Log::setID(int i){ id = i; }

// General
void Log::addParticipant(string p){
    participants.push_back(p);
    participantID = participants.size()-1;
}
void Log::removeParticpant(int pID){
   participants.erase(participants.begin() + pID);
} 
void Log::display(){
    string small_div = "--- ";
    cout << small_div << "LOG" << small_div << '\n'
    << "Date: " << date << "\n"
    << "Area: " << area << "\n";

}

// CAVELOG CLASS ========================================= //
string CaveLog::getName(){ return name; }
bool CaveLog::wasSRTCave(){ return srtCave; }
bool CaveLog::wasCaveLeader(){ return caveLeader; }
bool CaveLog::wasRigger(){ return rigger; }
void CaveLog::setName(string n){ name = n; }
void CaveLog::setSRTCave(bool b){ srtCave = b; }
void CaveLog::setCaveLeader(bool b){ caveLeader = b; }
void CaveLog::setRigger(bool b){ rigger = b; }

void CaveLog::display(){
    string small_div = "--- ";
    cout << small_div << " CAVE LOG " << small_div << '\n'
    << "Date: " << date << "\n"
    << "Cave: " << name << "\n"
    << "Area: " << area << "\n"
    << small_div << small_div << small_div << '\n';
}

CaveLog::CaveLog(string nm, string dt){
    setName(nm);
    setDate(dt);
}
CaveLog::CaveLog(string nm, string dt, string loc, string nt){
    setName(nm);
    setDate(dt);
    setArea(loc);
    setNote(nt);
}
CaveLog::CaveLog(string nm, string dt, string loc, string nt, vector<string> pts, bool srt, bool leader, bool rig){
    setName(nm);
    setDate(dt);
    setArea(loc);
    setNote(nt);
    setParticipants(pts);
    setSRTCave(srt);
    setCaveLeader(leader);
    setRigger(rig);
}

// HIKELOG CLASS ========================================= //
int HikeLog::getDist(){ return distance; }
string HikeLog::getWeather(){ return weather; }

void HikeLog::setDist(int d){ distance = d; }
void HikeLog::setWeather(string w){ weather = w; }

HikeLog::HikeLog(string dt){
    setDate(dt);
}
HikeLog::HikeLog(string dt, string nt){
    setDate(dt);
    setNote(nt);
}
HikeLog::HikeLog(string dt, string nt, int dist){
    setDate(dt);
    setNote(nt);
    setDist(dist);
}
HikeLog::HikeLog(string dt, string nt, int dist, string weth, vector<string> part){
    setDate(dt);
    setNote(nt);
    setDist(dist);
    setWeather(weth);
    setParticipants(part);
}