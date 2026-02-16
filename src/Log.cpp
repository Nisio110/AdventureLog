#include "../include/Log.h"

int Log::getDurationMins() {return durationMins;}
string Log::getLocation() {return location;}
vector<string> Log::getParticipants() {return participants;}
vector<string> Log::getNotes() {return notes;}
string Log::getDate() {return date;}

void Log::setDurationMins(int dMins){
    durationMins=dMins;
}
void Log::setDuration(int hours, int mins){
    durationMins=(hours*60)+mins;
}
void Log::setLocation(string l){
    location=l;
}
void Log::setParticipants(vector<string> ps){
    participants=ps;
}
void Log::setNotes(vector<string> ns){
    notes=ns;
}
void Log::setDate(string d){
    date=d;
}
void Log::addParticipant(string p){
    participants.push_back(p);
}
void Log::removeParticpant(string p){
    ;
} 
