#include "../include/Disk.h"
#include <iostream>

int main(){
	Disk d;
	d.initProgram();
	//d.printUserDetails();
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