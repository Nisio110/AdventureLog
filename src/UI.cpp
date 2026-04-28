#include "../include/UI.h"
#include <iostream>
#include <limits>
#include <cmath>
#include <exception>

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
            case SIGNUP:
                pageSelect = placeholder;
                do{signedUp = signUp();
                } while(!signedUp);
                break;
            case EXIT:
                pageSelect = placeholder;
                if (quitMenu()) quit = quitMenu();
                break;
            case LOGIN:
                pageSelect = placeholder;
                do{ loggedIn = logIn();
                } while (!loggedIn);

                if (loggedIn) {
                    bool mainMenuLoop{false};
                    do {
                        switch (mainMenu()){
                            case VIEW_LOGS: 
                                mainMenuLoop = logMenu();
                                break;
                            case ADD_LOG: 
                                mainMenuLoop = logCreator();
                                break;
                            // case SETTINGS: 
                            //     mainMenuLoop = userSettings();
                            //     break;
                            case LOGOUT: 
                                quit = quitMenu();
                                break;
                        }
                    } while (mainMenuLoop);
                }
                break;
        }
    }
}

// MENUS ========================================= //
bool UI::quitMenu(){
    printHeader("Exit Menu");
    bool confirmation {false};
    takeBoolInput("Are you sure? [Y/n]: ", confirmation);
    if (confirmation) {std::exit(0);}
    return true;
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
    return false;
}

bool UI::signUp(){
    std::string username;
    std::string password;
    std::string passwordCheck;
    
    printHeader("Account Creator");
    do {
        takeInput("Enter a username: ", username);
        if (!s.isUniqueUsername(username)){
            printErr("Username is taken");
        }
    } while (!s.isUniqueUsername(username));

    do {
        takeInput("Create a password: ", password);
        takeInput("Confirm password: ", passwordCheck);
        if (password != passwordCheck)
            {printErr("Passwords do not match");}
    } while (password != passwordCheck);

    try { s.createUser(username, password);
    } catch (std::runtime_error& e) {
        printErr(e.what());
        return false;
    }
    s.save();
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
                  //<< SETTINGS  << ": User Settings" << '\n'
                  << LOGOUT    << ": Logout"        << '\n';
        takeUIntInput("Select Option: ", pageChoice);
        if (pageChoice >= numChoices){
            printErr("Invalid page choice");
            std::cout << '\n';
        }
    } while(pageChoice >= numChoices);
    return pageChoice;
}

bool UI::logMenu(){
    using enum Logs;
    User* user = s.getCurrentUser();
    size_t selectedChoice{0};
    size_t log{0};
    const short shownLogs{6};
    size_t currentPage{1};
    bool loop {true};

    while(loop){
        std::vector<Log*> logs {user->getLogs()};
        size_t numPages = std::ceil((user->getLogs().size()) / shownLogs);
        size_t numLogs {logs.size()};

        printHeader("Logs Overview");
        log = (currentPage - 1) * shownLogs; // tie log selector to the current page.
        std::cout << "- Page " << currentPage << '\n';
        if (log < numLogs)
            {std::cout << LOG1 << ": Log " << logs.at(log)->getDate() << '\n';}
        else std::cout << LOG1 << ": - - -\n";
        if (log+1 < numLogs)
            {std::cout << LOG2 << ": Log " << logs.at(++log)->getDate() << '\n';}
        else std::cout << LOG2 << ": - - -\n";
        if (log+1 < numLogs)
            {std::cout << LOG3 << ": Log " << logs.at(++log)->getDate() << '\n';}
        else std::cout << LOG3 << ": - - -\n";
        if (log+1 < numLogs)
            {std::cout << LOG4 << ": Log " << logs.at(++log)->getDate() << '\n';}
        else std::cout << LOG4 << ": - - -\n";
        if (log+1 < numLogs)
            {std::cout << LOG5 << ": Log " << logs.at(++log)->getDate() << '\n';}
        else std::cout << LOG5 << ": - - -\n";
        if (log+1 < numLogs)
            {std::cout << LOG6 << ": Log " << logs.at(++log)->getDate() << '\n';}
        else std::cout << LOG6 << ": - - -\n";

        std::cout << SORT_MENU << ": Sort logs\n";
        std::cout << MAIN_MENU << ": Back to main menu\n";
        std::cout << PREV_PAGE << ": Previous page\n";
        std::cout << NEXT_PAGE << ": Next page\n";
        takeUIntInput("Select an option: ", selectedChoice);

        log = (currentPage - 1) * shownLogs; // tie log selector to the current page.
        switch (selectedChoice){
            case LOG1: 
                if (!viewLog(LOG1)) {loop = false;}
                break;
            case LOG2:
                if (!viewLog(LOG2)) {loop = false;}
                break;
            case LOG3:
                if (!viewLog(LOG3)) {loop = false;}
                break;
            case LOG4:
                if (!viewLog(LOG4)) {loop = false;}
                break;
            case LOG5:
                if (!viewLog(LOG5)) {loop = false;}
                break;
            case LOG6:
                if (!viewLog(LOG6)) {loop = false;}
                break;
            case NEXT_PAGE: 
                ++currentPage;
                break;
            case PREV_PAGE: 
                if (currentPage > 1) { --currentPage; }
                break;
            case MAIN_MENU: 
                return true;
                break;
            case SORT_MENU: 
                sortLogs();
                break;
            default: 
                printErr("Invalid selection");
        }
    }
    return true;
}

bool UI::userSettings(){
    int choice;
    cout << "User Settings"        << endl
         << "1: Change Username"   << endl
         << "2: Change Password"   << endl
         << "3: Delete Account"    << endl
         << "4: Back to Main Menu" << endl
         << "Select Option: ";
    cin >> choice;
    return true;

    // TODO: Add switch statement for each case.
}

bool UI::viewLog(size_t logSelect){
    using enum ViewLog;
    printHeader("Log Viewer");
    std::vector<Log*> logs = s.getCurrentUser()->getLogs();
    if(logSelect < logs.size()){
        logs.at(logSelect)->print();
    }
    
    std::cout << "- Options\n";
    //std::cout << EDIT_LOG << ": Edit log\n";
    //std::cout << DELETE_LOG << ": Delete log\n";
    std::cout << LOG_OVERVIEW << ": Back to log overview\n";
    std::cout << MAIN_MENU2 << ": Back to main menu\n";

    size_t selectedChoice{0};
    size_t numChoices{2};
    do{
        takeUIntInput("Select option: ", selectedChoice);
    } while (selectedChoice > numChoices);

    switch (selectedChoice){
        case LOG_OVERVIEW:
            return true;
        //case EDIT_LOG:
        //    editLog();
        //case DELETE_LOG:
        //   deleteLog();
        case MAIN_MENU2:
            return false;
    }

    // TODO: Add options
    // 0) Go back to log overview
    // 1) Edit log
    // 2) Delete log
    // 3) Exit program
    return true;
}

bool UI::logCreator(){
    // = Structure =
    // Leave blank if N/A
    // - for both -
    // Adventure Type: <Hike or Cave>
    // Date: <date>
    // Area: <area>
    // - if cave -
    // Cave name: <name>
    // Did you lead any part of the cave? [y/n]: <ans>
    // Was it an SRT cave? [y/n]: <ans>
    // Did you rig anything? [y/n]: <ans>
    // ---
    // - if hike -
    // Hike distance: <dist>
    // Describe the weather: <weather info>
    // ---
    // - for both -
    // Write a note: <note>

    printHeader("Log Creator");
    std::string type;
    std::string date;
    std::string area;
    std::size_t durMins{0};
    // - if cave -
    std::string cname;
    bool didLead {false};
    bool didSRT {false};
    bool didRig {false};   
    // - if hike -
    size_t dist{0};
    std::string weather;
    // - for both -
    bool addParticipants {false};
    std::vector<std::string> pNames;
    std::string note;

    do{
        takeInput("Choose log type [Hike/Cave]: ", type);
        if (type.empty()){printErr("Type cannot be empty");}
        else if (type != CaveLog::type && type != HikeLog::type){
            printErr("Invalid type");
        }
    } while (type != CaveLog::type && type != HikeLog::type);

    do {
        takeInput("Log date [DD/MM/YYYY]: ", date);
        if (date.empty()) {printErr("Date cannot be empty");}
    } while (date.empty());
    
    takeUIntInput("Duration [mins]: ", durMins);
    takeInput("Area: ", area);
    if (type == CaveLog::type){
        do {
            takeInput("Cave name: ", cname);
            if (cname.empty()){printErr("Cave name cannot be empty");}
        } while (cname.empty());
        takeBoolInput("Did you lead any part of the cave? [y/n]: ", didLead);
        takeBoolInput("Did you do any SRT? [y/n]: ", didSRT);
        takeBoolInput("Did you rig any part of the cave? [y/n]: ", didRig);
    }
    else if (type == HikeLog::type){
        takeUIntInput("Hike distance [km]: ", dist);
        takeInput("Describe the weather: ", weather);
    }
    takeBoolInput("Add participants? [y/n]: ", addParticipants);
    size_t numParticipants{0};
    if (addParticipants){
        takeUIntInput("How many? : ", numParticipants);
        for (size_t i{0}; i < numParticipants; ++i){
            std::string pName;
            const size_t threshold {2};
            do{ 
                std::cout << "Participant " << i + 1 << " name: ";
                takeInput("", pName);
                if (pName.empty()) {printErr("Participant name cannot be empty");}
            } while (pName.empty());

            if (pName.size() < threshold){loop = false;}
            else {
                pNames.push_back(pName);
                continue;
            }
        }
    }
    takeInput("Write a note: ", note);

    

    Log* log{nullptr};

    if (type == CaveLog::type){
        CaveLog* clog = new CaveLog();
        clog->setName(cname);
        clog->setCaveLeader(didLead);
        clog->setSRTCave(didSRT);
        clog->setRigger(didRig);
        log = clog;
    }
    else if (type == HikeLog::type){
        HikeLog* hlog = new HikeLog();
        hlog->setDist(dist);
        hlog->setWeather(weather);
        log = hlog;
    }
    log->setUserId(s.getCurrentUser()->getId());
    log->setDate(date);
    log->setDurationMins(durMins);
    log->setArea(area);

    std::vector<Participant*> pVec;    
    for (size_t i{0}; i < pNames.size(); ++i){
        Participant* p = new Participant(pNames.at(i), log->getId());
        pVec.push_back(p);
    }
    log->setParticipants(pVec);
    log->setNote(note);

    s.getCurrentUser()->addLog(log);
    s.save();
    return true;
}

void UI::editLog(){
    // Placeholder
}

void UI::deleteLog(){
    // Placeholder
}

void UI::openPage(size_t pageNum, size_t oldPage){
    switch (pageNum){
        case 1: 
            startupMenu();
            break;
        case 2:
            mainMenu();
            break;
        case 3:
            logMenu();
            break;
        case 4:
            quitMenu();
            break;
    }
    if (oldPage != 0){
        openPage(oldPage);
    }
}


// INPUT HANDLING =============================== //
std::string UI::takeInput(std::string_view prefix){
    std::string buffer;
    print(prefix);
    std::getline(std::cin, buffer);
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
