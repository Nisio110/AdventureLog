//#include "../include/DiskUsers.h"
#include "../include/State.h"
#include <iostream>
#include "../include/UI.h"
using namespace std;

int ui(){
    State s;
    s.initProgram();
    std::vector<User*> users = s.getUsers();
    int choice=-1;
    int log=-1;
    int page=1;
    bool loop=1;
    int menu=0;
    while (loop==1) {
        switch (menu) {
            case 0:
                choice = startupMenu();
                switch (choice){
                    case 1:
                        if (logIn()) { //some bool stuff here will need tweaks when the actual loginchecks are implemented
                            menu=1;
                        }
                        else{menu=0;}
                        break;
                    case 2:
                        signUp();
                        menu=1;
                        break;
                    case 3:
                        s.save();
                        return 0;
                        break;
                    default:
                        cout << "**Invalid Input: Please choose a valid option**" << endl;
                }
                break;
    
            case 1:
                choice = mainMenu();
                switch(choice){
                    case 1: menu=2;
                    case 2: menu=3;
                    case 3: menu=0;
                    default:cout << "**Invalid Input: Please choose a valid option**" << endl;
                }
                break;
            case 2:
                choice = logMenu(page);
                log=choice;
                switch(choice){
                    case 1: loadLog(log);
                    case 2: loadLog(log);
                    case 3: loadLog(log);
                    case 4: loadLog(log);
                    case 5: loadLog(log);
                    case 6: loadLog(log);
                    case 7: page++;
                    case 8: if (page>1) {page--;}
                    case 9: menu=1;
                    case 0: sortLogs();
                    default:cout << "**Invalid Input: Please choose a valid option**" << endl;
                }
                break;


        }
    }
    cout << "Good job you deserve a cookie";
    return 0;
}



