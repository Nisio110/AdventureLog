#include "../include/Log.h"
#include <iostream>
#include <string>
using std::string;
using std::cout;

// Getters
int Log::getDurationMins() {return durationMins;}
string Log::getLocation() {return location;}
vector<string> Log::getParticipants() {return participants;}
vector<string> Log::getNotes() {return notes;}
string Log::getDate() {return date;}
int Log::getID(){ return id; }

// Setters
void Log::setDurationMins(int dMins){ durationMins = dMins; }
void Log::setDuration(int hours, int mins){ durationMins = (hours*60) + mins; }
void Log::setLocation(string l){ location = l; }
void Log::setParticipants(vector<string> ps){ participants = ps; }
void Log::setNotes(vector<string> ns){ notes = ns; }
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
    << "Location: " << location << "\n";

}

// CaveLog
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
    cout << small_div << "LOG " << small_div << '\n'
    << "Date: " << date << "\n"
    << "Location: " << location << "\n"
    << small_div << small_div << small_div << '\n';
}

CaveLog::CaveLog(string nm, string dt){
    setName(nm);
    setDate(dt);
}
CaveLog::CaveLog(string nm, string dt, string loc, vector<string> nts){
    setName(nm);
    setDate(dt);
    setLocation(loc);
    setNotes(nts);
}
CaveLog::CaveLog(string nm, string dt, string loc, vector<string> nts, vector<string> pts, bool srt, bool leader, bool rig){
    setName(nm);
    setDate(dt);
    setLocation(loc);
    setNotes(nts);
    setParticipants(pts);
    setSRTCave(srt);
    setCaveLeader(leader);
    setRigger(rig);
}