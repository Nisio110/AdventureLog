#include "../include/Participant.h"
#include <iostream>
#include <string>
#include "../include/Tests.h"

int Participant::numParticipants{0};

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
    setNumParticipants(numParticipants+1);
    id = getNumParticipants();
    return id;
}

// Constructors
Participant::Participant(string name){
    setName(name);
    generateID();
}

// Tests
void ParticipantTests::testConstructors(){
    using namespace Tests;
    using std::cout;

    Participant p1("John Doe");
    Participant p2("Mary Jane");
    Participant p3("Alex Dartmouth");
    Participant p4("Sarah Bucksworth");
    int a{4}, b{1};
    
    cout << div(b) << " TESTING PARTICIPANT CONSTRUCTORS " << div(b) << '\n'
         << div(a) << '\n'
         << "p1.getID()" << ret << p1.getID() << "\t" << "p1.getName()" << ret << p1.getName() << '\n'
         << "p2.getID()" << ret << p2.getID() << "\t" << "p2.getName()" << ret << p2.getName() << '\n'
         << "p3.getID()" << ret << p3.getID() << "\t" << "p3.getName()" << ret << p3.getName() << '\n'
         << "p4.getID()" << ret << p4.getID() << "\t" << "p4.getName()" << ret << p4.getName() << '\n'
         << div(a) << '\n';
}