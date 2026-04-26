#include "../include/UI.h"
#include <iostream>
#include <limits>

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
        switch (pageSelect){
            using enum Startup;
            case LOGIN:
                pageSelect = placeholder;
                logIn();
            case SIGNUP:
                pageSelect = placeholder;
                signUp();
                break;
            case EXIT:
                pageSelect = placeholder;
                quit = exit();
                break;
        }

    }
}

// MENUS ========================================= //
// void UI::choosePage(int page){
//     switch (page){
//         case Page::STARTUP: 
//             startupMenu();
//             break;
//         case Page::MAIN:
//             mainMenu();
//             break;
//         case Page::LOGS:
//             logMenu();
//             break;
//         case Page::SIGNUP:
//             signUp();
//             break;
//         case Page::LOGIN:
//             logIn();
//             break;
        
//     }
// }

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

        if (pageChoice > numChoices) {
            printErr("Invalid page choice");
            std::cout << '\n';
        }
    } while (pageChoice > numChoices);
    return pageChoice;
}

int UI::mainMenu(){
    int choice;
    cout << "Main Menu"        << endl
         << "1: View Logs"     << endl
         << "2: Add Logs"      << endl
         << "3: User Settings" << endl
         << "4: Logout"        << endl
         << "Select Option: ";
    cin >> choice;
    return choice;
    
    // TODO: Add switch statement for each case
}

int UI::logMenu(){
    int choice{-1};
    int log{0}; // placeholder
    switch (choice){
        case 1: loadLog(log);
        case 2: loadLog(log);
        case 3: loadLog(log);
        case 4: loadLog(log);
        case 5: loadLog(log);
        case 6: loadLog(log);
        case 7: page++;
        case 8: if (page > 1) { page--; }
        case 9: menu = 1;
        case 0: sortLogs();
        default: cout << "**Invalid Input: Please choose a valid option**" << endl;
    }
    cout << "Page: " << page << endl;
    return choice;

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

// AUTH ========================================= //
void UI::logIn(){
    printHeader("Login Menu");
    std::string username;
    std::string password;

    takeInput("Enter username: ", username);
    takeInput("Enter password: ", password);

    try {
        if (s.logIn(username,password)) {mainMenu();}
    } catch (std::runtime_error& e){
        printErr(e.what());
        logIn();
    }
}

void UI::signUp(){
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
        signUp();
    }

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

void UI::loadLog(int logSelect){

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

