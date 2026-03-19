//#include "../include/DiskUsers.h"
#include <iostream>
using namespace std;

int main(){
    int choice=-1;
    int moveon=-1;
    while (moveon=!1){
        cout << "Welcome to the Adventure Log System" << endl << "Please choose an option" << endl << "1: Log in" << endl << "2: Sign up" << endl << "3: Exit";
        cin >> choice;
        if (choice==1){
            cout << "Please enter your username: ";
            cout << endl;
            cout << "Please enter your password: ";
            cout << endl;
            moveon=1;
        }
        else if (choice==2){
            cout << "Please choose a username: ";
            cout << endl;
            cout << "Please choose a password: ";
            cout << endl;
            moveon=1;
        }
        else if (choice==3){return 0;}
    }
    moveon=0;





    cout << "Good job you deserve a cookie";
    return 0;
}