#include "../include/User.h"


int main(){
    User u1("oisin", "password");
    CaveLog c1 ("Bruce's Pot", "16/02/2026");
    c1.setArea("Fermanagh");
    c1.setSRTCave(true);
    c1.setCaveLeader(false);
    c1.setRigger(false);
    c1.display();

    return 0;
}