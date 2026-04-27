#include "../include/UI.h"

int main(int argc, char* argv[]){
	std::string path;
	if (argc > 1){ path = argv[1]; }
	UI ui(path);
	try{
		ui.run();
	} catch (std::exception& e){}
	return 0;
}
