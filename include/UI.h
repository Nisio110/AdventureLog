#ifndef UI_H
#define UI_H
int ui();

int startupMenu()
{
    int choice;
    cout << "Welcome to the Adventure Log System" << endl 
         << "Please choose an option" << endl
         << "1: Log in" << endl
         << "2: Sign up" << endl
         << "3: Exit" << endl
         << "Select Option: ";
    cin >> choice;
    return choice;
};

bool logIn()
{
    cout << "Please enter your username: ";
    cout << endl;
    cout << "Please enter your password: ";
    cout << endl;
    return true;
}

void signUp()
{
    logIn(); //disk stuff that need to be done
}

int mainMenu()
{
    int choice;
    cout << "Main Menu" << endl
         << "1: View Logs" << endl
         << "2: Add Logs" << endl
         << "3: User Settings" << endl
         << "4: Logout" << endl
         << "Select Option: ";
    cin >> choice;
    return choice;
}

int logMenu(int page)
{
    int choice;
    int page=page;

    cout << "Page: " << page << endl;
}

void loadLog(int log){}

void sortLogs(){}

void editLog(){}

void deleteLog(){}

void addLog(){}

int userSettings()
{
    int choice;
    cout << "User Settings" << endl
         << "1: Change Username" << endl
         << "2: Change Password" << endl
         << "3: Delete Account" << endl
         << "4: Back to Main Menu" << endl
         << "Select Option: ";
    cin >> choice;
    return choice;
}

void changeUsername(){}

void changePassword(){}

void deleteAccount(){}




vector<Log*> sortID(vector<Log*> logs);
vector<Log*> sortDuration(vector<Log*> logs);


#endif