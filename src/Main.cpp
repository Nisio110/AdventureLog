#include "../include/DiskUsers.h"
#include <iostream>

int main(){
	Disk d;
	std::vector<std::pair<std::string, std::string>> v {d.parseFile()};
	for (auto pair : v){
		std::cout << std::get<0>(pair) << " -> " << std::get<1>(pair) << "\n";
	}
	//size_t i{3};
	//d.parseStr(i);
	
	//d.parseObjectBody("User",1);
	
	/*
	std::string targetAttr {"id"};
	std::string targetValue("1");
	std::vector<size_t> attrLocs = d.parseAttrLocs(targetAttr);
	for (int i{};i<size(attrLocs);++i)
		std::cout << attrLocs[i]+1 << "\n";
	*/
	/*
	du.parseObjectLocs();
	for (int i{};i<size(du.getObjectLocs()); ++i)
		std::cout << du.getObjectLocs()[i] << std::endl;
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