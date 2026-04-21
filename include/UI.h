#ifndef UI_H
#define UI_H

int ui();
int startupMenu();
bool logIn();
void signUp();
int mainMenu();
int logMenu(int page);
void loadLog(int log);
void sortLogs();
void editLog();
void deleteLog();
void addLog();
int userSettings();
void changeUsername();
void changePassword();
void deleteAccount();
std::vector<Log*> sortID(vector<Log*> logs);
std::vector<Log*> sortDuration(vector<Log*> logs);

#endif