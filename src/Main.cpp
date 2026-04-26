#include "../include/UI.h"
#include <iostream>

int main(int argc, char* argv[]){
	std::string path;
	if (argc > 1){ path = argv[1]; }
	State s(path);
}