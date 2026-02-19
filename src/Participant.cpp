#include "../include/Participant.h"

// Getters
int Participant::getID(){ return id; }
string Participant::getName(){ return name; }
int Participant::getNumParticipants(){ return numParticipants; }

// Setters
void Participant::setID(int i){ id = i; }
void Participant::setName(string n){ name = n; }
void Participant::setNumParticipants(int num){ numParticipants = num; }

// General
int Participant::generateID(){
    id = getNumParticipants() + 1;
    return id;
}

// Constructors
Participant::Participant(string name){
    setName(name);
    generateID();
}