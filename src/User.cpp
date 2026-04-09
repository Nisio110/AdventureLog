#include "../include/User.h"
#include <iostream>
using std::string;
using std::vector;

int User::numUsers{0};

// Getters & Setters
int User::getId(){ return id; }
string User::getName(){return name;}
string User::getPasswd(){return passwd;}
vector<Log*> User::getLogs(){return logs;}

void User::setID(int i){id = i;}
void User::setName(string n){name = n;}
void User::setPasswd(string p){passwd = p;}
void User::setLogs(vector<Log*> l){logs = l;}

void User::generateID(){
    numUsers += 1;
    id = numUsers;
}
User::User(){}

User::User(string n){
    generateID();
    setName(n);
}
User::User(string n, string p){
    generateID();
    setName(n);
    setPasswd(p);
}

// General
void User::print(){
    std::cout << "User: " << getName() << "\n";
}
void User::printAll(){
    print();
    std::cout << "=== LOGS ==================" << "\n";
    for (auto log : logs)
        { log->print(); }
}

void User::addLog(Log* log){
    logs.push_back(log);
}
void User::removeLog(int id){
    for (size_t i{0}; i < logs.size(); ++i){
        if (logs.at(i)->getID() == id) logs.erase(logs.begin() + i);
    }
} // pass in objects id variable 

void UserTests::testCaveLogsIO(){}
void UserTests::testHikeLogsIO(){}
void UserTests::testConstructors(){}
void UserTests::testGenerateID(){}