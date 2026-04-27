#include "../include/UI.h"

int main(int argc, char* argv[]){
	if (argc > 1){
		UI ui(argv[1]);
		ui.run();
	} else {
		UI ui;
		ui.run();
	}
	return 0;
}
