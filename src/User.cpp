#include "../include/User.h"
#include <iostream>
using std::string;
using std::vector;
<<<<<<< HEAD
=======

>>>>>>> origin/main
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
void User::seedIdCounter(int n){ numUsers = n; }
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
    std::cout << "\n";
}

void User::addLog(Log* log){
    logs.push_back(log);
}
void User::removeLog(Log* l){
    for (size_t i{0}; i < logs.size(); ++i){
        auto log = logs.at(i);
        if (log->getId() == l->getId()) {
            delete log;
            logs.erase(logs.begin() + i);
        }
    }
}

void UserTests::testCaveLogsIO(){}
void UserTests::testHikeLogsIO(){}
void UserTests::testConstructors(){}
void UserTests::testGenerateID(){}