#include "../include/User.h"

int main(){
    ParticipantTests::testConstructors();
    return 0;
}

void basicTest(){
    User u1("oisin", "password");
    CaveLog c1 ("Bruce's Pot", "16/02/2026");
    c1.setArea("Fermanagh");
    c1.setSRTCave(true);
    c1.setCaveLeader(false);
    c1.setRigger(false);
    c1.display();
}