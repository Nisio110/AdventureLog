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
    bool logIn(); 
    bool signUp();
    bool exit(); // signals to the run() function that it should end

    //void choosePage(int menu);
    void viewLog(size_t logSelector);
    void sortLogs();
    void editLog();
    void deleteLog();
    void addLog();
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
        LOGIN,
        SIGNUP,
        EXIT,
    };

    enum Main{
        VIEW_LOGS,
        ADD_LOG,
        SETTINGS,
        LOGOUT
    };

    enum Logs{
        LOG1,
        LOG2,
        LOG3,
        LOG4,
        LOG5,
        LOG6,
        NEXT_PAGE,
        PREV_PAGE,
        MAIN_MENU,
        SORT_MENU
    }
};


#endif
