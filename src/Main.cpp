#include "../include/State.h"
#include <iostream>
<<<<<<< HEAD
#include "../include/UI.h"

int main(){
	ui();
	return 0;
=======

int main(){
	State s;
	Disk d;
	s.initProgram();
	
	// Add a new user
	std::string name;
	std::cout << "Enter a username: ";
	std::cin >> name;
	User u(name);
	Log l1(u.getId(),"10/04/2026", "This is a test!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
	s.addUser(&u);
	s.printAll();
	s.save();
	//d.printLogDetails();
	//d.printUserDetails();
	//d.printParticipantDetails();
>>>>>>> origin/main
}


void basicTest(){
	User u1("oisin", "password");
	//User* u2;
	CaveLog c1(u1.getId(),"Bruce's Pot", "16/02/2026");
	c1.setArea("Fermanagh");
	c1.setSRTCave(true);
	c1.setCaveLeader(false);
	c1.setRigger(false);
	c1.print();
}