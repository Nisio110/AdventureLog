#include "../include/Log.h"
#include "../include/User.h"


int main(){
    User u1("oisin", "password");
    CaveLog c1 ("Bruce's Pot", "16/02/2026");
    c1.setSRTCave(true);
    c1.setCaveLeader(false);
    c1.setRigger(false);

    return 0;
}