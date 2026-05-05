#ifndef UI_H
#define UI_H
#include "State.h"

class UI {
    State s;
    User u;

public:
    UI(std::string path = State::defaultDiskPath);
    void run();

private:

    // Menus: Select a page to navigate to
    int startupMenu();
    int mainMenu();
    bool logMenu();

    // pages
    bool quitMenu(); // signals to the run() function that it should end
    bool logIn(); 
    bool signUp();
    bool viewLog(size_t logSelector);
    bool logCreator();
    void deleteLog();
    void editLog();

    //void choosePage(int menu);
    void sortLogs();
    bool userSettings();
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
    static size_t takeMCQ(std::vector<std::string> choices);
    void openPage(size_t pageNum, size_t oldPage = 0);

public:
    // Sorting — pure utilities on a Log* vector, exposed for unit testing.
    static std::vector<Log*> sortID(std::vector<Log*> logs);
    static std::vector<Log*> sortDuration(std::vector<Log*> logs);

private:

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
        VIEW_LOGS,
        ADD_LOG,
        //SETTINGS,
        LOGOUT
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
        //EDIT_LOG = 0,
        //DELETE_LOG = 1,
        LOG_OVERVIEW,
        MAIN_MENU2
    };
};


#endif
