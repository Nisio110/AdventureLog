#include "../include/Disk.h"
#include "../include/DiskUsers.h"
#include <iostream>

int main(){
	DiskUsers du;
	du.parseLocations();
	for (auto lineNum : du.getObjectLocations())
		std::cout << "User object found at line: " << lineNum;
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