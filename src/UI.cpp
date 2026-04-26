#include "../include/UI.h"
#include <iostream>
#include <limits>
#include <cmath>

using std::cin;
using std::cout;
using std::endl;
using std::vector;

UI::UI(std::string path) : s(path) {}

// MAIN UI LOOP ========================================= //
void UI::run(){
    size_t placeholder {1000};
    size_t pageSelect{placeholder}; // buffer for holding page selection info
    bool quit {false};

    while (!quit) {
        pageSelect = startupMenu();
        bool loggedIn {false};
        bool signedUp {false};
        switch (pageSelect){
            using enum Startup;
            case LOGIN:
                pageSelect = placeholder;
                do{loggedIn = logIn();
                } while (!logIn());
                if (loggedIn) {mainMenu();}
                break;
            case SIGNUP:
                pageSelect = placeholder;
                do{signedUp = signUp();
                } while(!signUp());
                if (signedUp) {startupMenu();}
                break;
            case EXIT:
                pageSelect = placeholder;
                if (exit()) quit = exit();
                break;
        }

    }
}

// MENUS ========================================= //
bool UI::exit(){
    bool confirmation {false};
    printHeader("Exit");
    takeBoolInput("Are you sure? [Y/n]: ", confirmation);
    return confirmation;
}

int UI::startupMenu(){
    using enum Startup;
    size_t pageChoice;
    
    // Adjust according to current number of page choices!
    size_t numChoices{3};
    // Adjust according to current number of page choices!

    printHeader("Welcome to the Adventure Logging System");

    do {
        printl("Please choose an option");
        std::cout << LOGIN  << ": Log in"  << '\n'
                  << SIGNUP << ": Sign up" << '\n'
                  << EXIT   << ": Exit"    << '\n';

        takeUIntInput("Select option: ", pageChoice);

        if (pageChoice >= numChoices) {
            printErr("Invalid page choice");
            std::cout << '\n';
        }
    } while (pageChoice >= numChoices);
    return pageChoice;
}

// AUTH ========================================= //
bool UI::logIn(){
    printHeader("Login Menu");
    std::string username;
    std::string password;

    takeInput("Enter username: ", username);
    takeInput("Enter password: ", password);

    try {
        if (s.logIn(username,password)) {
            return true;
        }
    } catch (std::runtime_error& e){
        printErr(e.what());
        return false;
    }

}

bool UI::signUp(){
    std::string username;
    std::string password;
    std::string passwordCheck;
    
    printHeader("Account Creator");
    do {
        takeInput("Enter a username: ", username);
    } while (!s.isUniqueUsername(username));

    do {
        takeInput("Create a password: ", password);
        takeInput("Confirm password: ", passwordCheck);
        if (password != passwordCheck)
            {printErr("Passwords do not match");}
    } while (!(password != passwordCheck));

    try { s.createUser(username, password);
    } catch (std::runtime_error& e) {
        printErr(e.what());
        return false;
    }
    return true;
}

// MENUS ========================================= //
int UI::mainMenu(){
    using enum Main;
    size_t pageChoice{0};
    size_t numChoices{4};

    printHeader("Main Menu");
    do {
        std::cout << VIEW_LOGS << ": View Logs"     << '\n'
                  << ADD_LOG  << ": Add Log"      << '\n'
                  << SETTINGS  << ": User Settings" << '\n'
                  << LOGOUT    << ": Logout"        << '\n';
        takeUIntInput("Select Option: ", pageChoice);
        if (pageChoice >= numChoices){
            printErr("Invalid page choice");
            std::cout << '\n';
        }
    } while(pageChoice >= numChoices);
    return pageChoice;
}

int UI::logMenu(){
    using enum Logs;
    User* user = s.getCurrentUser();
    std::vector<Log*> logs {user->getLogs()};
    size_t selectedChoice{0};
    size_t log{0};
    const short shownLogs{6};
    size_t currentPage{1};
    size_t numPages = std::ceil((user->getLogs().size()) / shownLogs);


    printHeader("Logs Overview");
    
    log = (currentPage - 1) * shownLogs; // tie log selector to the current page.
    std::cout << LOG1 << ": " << *logs.at(log) << '\n'
              << LOG2 << ": " << *logs.at(++log) << '\n'
              << LOG3 << ": " << *logs.at(++log) << '\n'
              << LOG4 << ": " << *logs.at(++log) << '\n'
              << LOG5 << ": " << *logs.at(++log) << '\n'
              << LOG6 << ": " << *logs.at(++log) << '\n'
              << NEXT_PAGE << ": Next page\n"
              << PREV_PAGE << ": Previous page\n"
              << MAIN_MENU << ": Main menu\n"
              << SORT_MENU << ": Sort logs\n";
    takeUIntInput("Select an option", selectedChoice);

    log = (currentPage - 1) * shownLogs; // tie log selector to the current page.
    switch (selectedChoice){
        case LOG1: viewLog(log);
        case LOG2: viewLog(++log);
        case LOG3: viewLog(++log);
        case LOG4: viewLog(++log);
        case LOG5: viewLog(++log);
        case LOG6: viewLog(++log);
        case NEXT_PAGE: ++currentPage;
        case PREV_PAGE: if (page > 1) { page--; }
        case MAIN_MENU: mainMenu();
        case SORT_MENU: sortLogs();
        default: printErr("Invalid selection");
    }
    cout << "Page: " << page << '\n';
    
    return 0;

    // TODO: Figure out what the fuck is going on here
}

int UI::userSettings(){
    int choice;
    cout << "User Settings"        << endl
         << "1: Change Username"   << endl
         << "2: Change Password"   << endl
         << "3: Delete Account"    << endl
         << "4: Back to Main Menu" << endl
         << "Select Option: ";
    cin >> choice;
    return choice;

    // TODO: Add switch statement for each case.
}

void UI::viewLog(size_t logSelect){
    std::vector<Log*> logs = s.getCurrentUser()->getLogs();
    logs.at(logSelect)->print();
    // TODO: Add options
    // 0) Go back to log overview
    // 1) Edit log
    // 2) Delete log
    // 3) Exit program
}

// INPUT HANDLING =============================== //
std::string UI::takeInput(std::string_view prefix){
    std::string buffer;
    print(prefix);

    std::getline(std::cin >> std::ws, buffer);

    // try {
    //     std::getline(std::cin >> std::ws, buffer);
    // } catch (std::exception& e){
    //     printErr(e.what());
    //     takeInput(prefix);
    //     resetInputStream(std::cin);
    // }
    return buffer;
}

void UI::resetInputStream(std::istream& in){
    // Clear failbit/badbit flags
    in.clear();
    // Discard the rest of the stream
    in.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

std::size_t UI::takeUIntInput(std::string_view prefix){
    std::string buffer {takeInput(prefix)};
    const short base {10};
    std::size_t convertedUInt{0};

    try{
		convertedUInt = std::stoul(buffer.c_str(),nullptr, base);
	} catch (std::invalid_argument& e){
        printErr("Invalid argument");
        convertedUInt = takeUIntInput(prefix);
    } catch (std::exception& e){
        printErr("Exception reached");
        printErr(e.what());
        convertedUInt = takeUIntInput(prefix);
    }
    return convertedUInt;
}

bool UI::takeBoolInput(std::string_view prefix){
    std::string input {takeInput(prefix)};
    std::string preConversion;
    bool postConversion {false};
    if (input == "y" || input == "Y" || input == "Yes" || input == "yes"){
        preConversion = "true";
    }
    else if (input == "n" || input == "N" || input == "No" || input == "no"){
        preConversion = "false";
    }

    try{
        postConversion = DiskHelper::strToBool(preConversion);
    }
    catch(std::runtime_error& e){
        printErr(e.what());
        takeBoolInput(prefix);
    }
    return postConversion;
}

std::string UI::takeInput(std::string_view prefix, std::string& buffer){
    std::string input {takeInput(prefix)};
    buffer = input;
    return buffer;
}

std::size_t UI::takeUIntInput(std::string_view prefix, std::size_t& buffer){
    std::size_t input {takeUIntInput(prefix)};
    buffer = input;
    return buffer;
}

bool UI::takeBoolInput(std::string_view prefix, bool& buffer){
    bool input {takeBoolInput(prefix)};
    buffer = input;
    return buffer;
}

// SORTING ========================================= //
vector<Log*> UI::sortID(vector<Log*> inputs){
    vector<Log*> outputs;

    int size = inputs.size();
    int position = size;
    Log* pivot = inputs.at(0);
    for (position; position > 0; --position){
        for (int counter{1}; counter < position; ++counter){
            Log* test = inputs.at(counter);
            if (Log::sortByID(pivot, test));{
                pivot = test;
            }
        }
        outputs.at(position) = pivot;
    }
    return outputs;
}

vector<Log*> UI::sortDuration(vector<Log*> logs){
    vector<Log*> outputs;

    int size = logs.size();
    int position = size;
    Log* pivot = logs.at(0);
    for (position; position > 0; --position){
        for (int counter{1}; counter < position; ++counter){
            Log* test = logs.at(counter);
            if (Log::sortByDuration(pivot, test));{
                pivot = test;
            }
        }
        outputs.at(position) = pivot;
    }
    return outputs;
}


void UI::sortLogs(){

}


// PRINTING ======================================== //
void UI::print(std::string_view buffer){
    std::cout << buffer;
}

void UI::printl(std::string_view buffer){
    std::cout << buffer << '\n';
}

void UI::printErr(std::string_view buffer){
    DiskHelper::printErr(buffer);
}

void UI::printHeader(std::string_view buffer){
    std::cout << '\n' << "===" << " " << buffer << " " << '\n';
}

