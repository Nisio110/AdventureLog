#include "../include/Log.h"
#include <iostream>
using std::cout;

int Log::getDurationMins() {return durationMins;}
string Log::getLocation() {return location;}
vector<string> Log::getParticipants() {return participants;}
vector<string> Log::getNotes() {return notes;}
string Log::getDate() {return date;}
int Log::getID(){ return id; }

void Log::setDurationMins(int dMins){ durationMins = dMins; }
void Log::setDuration(int hours, int mins){
    durationMins = (hours*60) + mins;
}
void Log::setLocation(string l){ location = l; }
void Log::setParticipants(vector<string> ps){ participants = ps; }
void Log::setNotes(vector<string> ns){ notes = ns; }
void Log::setDate(string d){ date = d; }

void Log::addParticipant(string p){
    participants.push_back(p);
    participantID = participants.size()-1;
}
void Log::removeParticpant(int pID){
   participants.erase(participants.begin() + pID);
} 
void Log::display(){
    string small_div = "--- ";
    cout << small_div << LOG << small_div;

}