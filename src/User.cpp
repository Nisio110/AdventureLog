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
    int id;
    // log adding logic
    return id;
}
int User::addCaveLog(){
    int id{};
    // logic here
    return id;
}
int User::addHikeLog(){
    // logic here
    return id;
}
void User::removeCaveLog(int id){
    // logic here
}

void User::removeHikeLog(int id){
    // logic here
} 

User::User(){
    num_users += 1;
    id = num_users;
    name = "N/A";
    passwd = "password123";
}
User::User(string n){
    num_users += 1;
    id = num_users;
    setName(n);
    setPasswd("password123");
}
User::User(string n, string p){
    num_users += 1;
    id = num_users;
    setName(n);
    setPasswd(p);
}