#include "../include/Participant.h"
#include <iostream>
#include <string>
#include "../include/Tests.h"
using std::string;

int Participant::numParticipants{0};

// Getters
int Participant::getId(){ return id; }
string Participant::getName(){ return name; }
int Participant::getNumParticipants(){ return numParticipants; }

// Setters
void Participant::setID(int i){ id = i; }
void Participant::setName(string n){ name = n; }
void Participant::setNumParticipants(int num){ numParticipants = num; }

// General
void Participant::print(){
    std::cout << getId() << ") " << getName() << "\n";
}

int Participant::generateID(){
    setNumParticipants(numParticipants+1);
    id = getNumParticipants();
    return id;
}
void Participant::seedIdCounter(int n){ numParticipants = n; }

// Constructors
Participant::Participant(std::string name){
    setName(name);
    generateID();
}
Participant::Participant(std::string name, int logId){
    setName(name);
    setLogID(logId);
    generateID();
}

Participant::Participant(std::string name, int logId, int id){
    setName(name);
    setLogID(logId);
    setID(id);
}

// Tests
bool ParticipantTests::testConstructors(){
    using namespace Tests;
    using std::cout;

    Participant p1("John Doe");
    Participant p2("Mary Jane");
    Participant p3("Alex Dartmouth");
    Participant p4("Sarah Bucksworth");
    int a{4}, b{1};
    
    cout << div(b) << " TESTING PARTICIPANT CONSTRUCTORS " << div(b) << '\n'
         << div(a) << '\n'
         << "p1.getId()" << ret << p1.getId() << "\t" << "p1.getName()" << ret << p1.getName() << '\n'
         << "p2.getId()" << ret << p2.getId() << "\t" << "p2.getName()" << ret << p2.getName() << '\n'
         << "p3.getId()" << ret << p3.getId() << "\t" << "p3.getName()" << ret << p3.getName() << '\n'
         << "p4.getId()" << ret << p4.getId() << "\t" << "p4.getName()" << ret << p4.getName() << '\n'
         << div(a) << '\n';
    return verifyTest();
}