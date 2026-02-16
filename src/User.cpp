#include "../include/User.h"

// Getters & Setters
string User::getName(){return name;}
string User::getPasswd(){return passwd;}
vector<Cave> &User::getCaveLogs(){return caveLogs;}
vector<Hike> &User::getHikeLogs(){return hikeLogs;}

void User::setName(string n){name = n;}
void User::setPasswd(string p){passwd = p;}
void User::setCaveLogs(vector<Cave> cl){caveLogs = cl;}
void User::setHikeLogs(vector<Hike> hl){hikeLogs = hl;}

// General
int User::addCaveLog(){
    int id; // temp: object will have id variable
    // log adding logic
    return id;
}
int User::addCaveLog(){
    int id; // temp: object will have id variable
    // logic here
    return id;
}
int User::addHikeLog(){
    // logic here
    // temp: object will have id variable
    return id;
}
void User::removeCaveLog(int id){
    // logic here
} // pass in objects id variable 

void User::removeHikeLog(int id){
    // logic here
} // pass in objects id variable 

void User::createID(){
    num_users += 1;
    id = num_users;
}

User::User(){
    createID();
    setName("N/A");
    setPasswd("password123");
}
User::User(string n){
    createID();
    setName(n);
    setPasswd("password123");
}
User::User(string n, string p){
    createID();
    setName(n);
    setPasswd(p);
}