#include "../include/User.h"
int User::numUsers{0};

// Getters & Setters
int User::getID(){ return id; }
string User::getName(){return name;}
string User::getPasswd(){return passwd;}
vector<CaveLog> &User::getCaveLogs(){return caveLogs;}
vector<HikeLog> &User::getHikeLogs(){return hikeLogs;}

void User::setID(int i){id = i;}
void User::setName(string n){name = n;}
void User::setPasswd(string p){passwd = p;}
void User::setCaveLogs(vector<CaveLog> &cl){caveLogs = cl;}
void User::setHikeLogs(vector<HikeLog> &hl){hikeLogs = hl;}

void User::generateID(){
    numUsers += 1;
    id = numUsers;
}

User::User(){
    generateID();
}
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
void User::addCaveLog(CaveLog& c){
    caveLogs.push_back(c);
    c.setID(caveLogs.size()-1);
}
void User::addHikeLog(HikeLog &h){
    hikeLogs.push_back(h);
    h.setID(hikeLogs.size()-1);
    // logic here
    // temp: object will have id variable
}
void User::removeCaveLog(int id){
    caveLogs.erase(caveLogs.begin()+id);
} // pass in objects id variable 

void User::removeHikeLog(int id){
    hikeLogs.erase(hikeLogs.begin()+id);
} // pass in objects id variable 

void UserTests::testCaveLogsIO(){}
void UserTests::testHikeLogsIO(){}
void UserTests::testConstructors(){}
void UserTests::testGenerateID(){}