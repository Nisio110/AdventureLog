#include "../include/UI.h"
#include <iostream>

int main(int argc, char* argv[]){
	std::string path;
	if (argc > 1){ path = argv[1]; }
	try{
		State s(path);
	} catch (std::exception& e){
		std::cout << "Exception triggered: " << e.what() << '\n';
	}
}