#ifndef UI_H
#define UI_H
#include "Disk.h"
class State;

int ui(State& s);
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
std::vector<Log*> sortID(std::vector<Log*> logs);
std::vector<Log*> sortDuration(std::vector<Log*> logs);



#endif