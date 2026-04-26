#ifndef UI_H
#define UI_H
#include "State.h"

class UI {
    State s;
    User u;
    int menu {0};
    int page {1};
    bool loop {true};

public:
    UI(std::string path);
    void run();

private:

    // Menus: Select a page to navigate to
    int startupMenu();
    int mainMenu();
    int logMenu();

    // pages
    bool exit(bool forceQuit = 0); // signals to the run() function that it should end
    bool logIn(); 
    bool signUp();
    void viewLog(size_t logSelector);
    void logCreator();
    void deleteLog();
    void editLog();

    //void choosePage(int menu);
    void sortLogs();
    int userSettings();
    void changeUsername();
    void changePassword();
    void deleteAccount();

    // Input Handling
    static std::string takeInput(std::string_view prefix);
    static void resetInputStream(std::istream& i);

    static std::size_t takeUIntInput(std::string_view prefix);
    static bool takeBoolInput(std::string_view prefix);
    static std::string takeInput(std::string_view prefix, std::string& buffer);
    static std::size_t takeUIntInput(std::string_view prefix, std::size_t& buffer);
    static bool takeBoolInput(std::string_view prefix, bool& buffer);

    // Sorting
    static std::vector<Log*> sortID(std::vector<Log*> logs);
    static std::vector<Log*> sortDuration(std::vector<Log*> logs);

    // Printing
    static void print(std::string_view); 
    static void printl(std::string_view);
    static void printErr(std::string_view);
    static void printHeader(std::string_view);


    enum Startup{
        LOGIN = 0,
        SIGNUP = 1,
        EXIT = 2,
    };

    enum Main{
        VIEW_LOGS = 0,
        ADD_LOG = 1,
        SETTINGS = 2,
        LOGOUT = 3
    };

    enum Logs{
        LOG1 = 0,
        LOG2 = 1,
        LOG3 = 2,
        LOG4 = 3,
        LOG5 = 4,
        LOG6 = 5,
        SORT_MENU = 6,
        MAIN_MENU = 7,
        PREV_PAGE = 8,
        NEXT_PAGE = 9,
    };

    enum ViewLog{
        LOG_OVERVIEW,
        EDIT_LOG,
        DELETE_LOG,
        EXIT_PROGRAM
    };
};


#endif
