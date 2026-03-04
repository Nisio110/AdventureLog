#include "../include/User.h"
#include "../include/UI.h"

int main(){
    ui();
    //ParticipantTests::testConstructors();
    //LogTests::testCaveLogConstructors();
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