#include "../include/User.h"
#include "Log.h"

// Getters & Setters
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
    num_users += 1;
    id = num_users;
}

User::User(){
    generateID();
    setName("N/A");
    setPasswd("password123");
}
User::User(string n){
    generateID();
    setName(n);
    setPasswd("password123");
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