//#include "../include/DiskUsers.h"
#include <iostream>
#include "../include/UI.h"
using namespace std;

int ui(){
    int choice=-1;
    bool loop=1;
    int menu=0;
    while (loop==1) {
        switch (menu) {
            case 0:
                cout << "Welcome to the Adventure Log System" << endl 
                     << "Please choose an option" << endl
                     << "1: Log in" << endl
                     << "2: Sign up" << endl
                     << "3: Exit" << endl;
                cin >> choice;
                switch (choice){
                    case 1:
                        cout << "Please enter your username: ";
                        cout << endl;
                        cout << "Please enter your password: ";
                        cout << endl;
                        menu=1;
                        break;
                    case 2:
                        cout << "Please choose a username: ";
                        cout << endl;
                        cout << "Please choose a password: ";
                        cout << endl;
                        menu=1;
                        break;
                    case 3:
                        return 0;
                        break;
                    default:
                        cout << "**Invalid Input: Please choose a valid option**" << endl;
                }
                break;
    
            case 1:
                cout << "Main Menu" << endl
                     << "1: View Logs" << endl
                     << "2: Add Logs" << endl
                     << "3: Logout" << endl;
                cin >> choice;
                switch(choice){
                    case 1: menu=2;
                    case 2: menu=3;
                    case 3: menu=0;
                    default:cout << "**Invalid Input: Please choose a valid option**" << endl;
                }
                break;
            case 2:
                cout << endl;


        }
    }
    cout << "Good job you deserve a cookie";
    return 0;
}