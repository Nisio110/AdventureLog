#include "../include/io.h"

int main(){
    IOTests::fileReadTest();
    /*
    User u1("Alex Dario");
    ParticipantTests::testConstructors();
    LogTests::testCaveLogConstructors(&u1);
    */
    return 0;
}

void basicTest(){
    User u1("oisin", "password");
    //User* u2;
    CaveLog c1(&u1,"Bruce's Pot", "16/02/2026");
    c1.setArea("Fermanagh");
    c1.setSRTCave(true);
    c1.setCaveLeader(false);
    c1.setRigger(false);
    c1.display();
}