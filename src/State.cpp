#include "../include/State.h"
#include <iostream>

void State::setUsers(std::vector<User*> u){
    users = u;
}

void State::loadSave(std::string path){
    disk.loadFromDisk(path);
    users = disk.getUsers();
}

void State::addUser(User* u){
    users.push_back(u);
}

void State::printAll(){
    for (auto u : users){
        u->printAll();
    }
}

void State::removeUser(User* u){
    for (size_t i{0}; i < users.size(); ++i){
        if (users.at(i)->getId() == u->getId()) {
            users.erase(users.begin() + i);
        }
    }
    disk.removeUser(u);
}

void State::save(Disk& disk){
    using VecString = std::vector<std::string>;
    std::vector<VecString> _users;
    std::vector<VecString> _logs;
    std::vector<VecString> _parts;

	for (auto u : users){
		_users.push_back(disk.userToStr(u));
        for (auto log : u->getLogs()){
            _logs.push_back(disk.logToStr(log));
            for (auto p : log->getParticipants()){
                _parts.push_back(disk.partToStr(p));
            }
        }
	}

	std::vector<std::string> buffer;
	for (auto vec : _users)
		buffer.insert(buffer.end(), vec.begin(), vec.end());
	for (auto vec : _logs)
		buffer.insert(buffer.end(), vec.begin(), vec.end());
	for (auto vec : _parts)
		buffer.insert(buffer.end(), vec.begin(), vec.end());

	disk.writeToDisk(buffer);
}

void State::save(){
    using VecString = std::vector<std::string>;
    std::vector<VecString> _users;
    std::vector<VecString> _logs;
    std::vector<VecString> _parts;

	for (auto u : users){
		_users.push_back(disk.userToStr(u));
        for (auto log : u->getLogs()){
            _logs.push_back(disk.logToStr(log));
            for (auto p : log->getParticipants()){
                _parts.push_back(disk.partToStr(p));
            }
        }
	}

    // Error checking
    if (disk.getUsers().size() != users.size()){
        DiskHelper::printErr("State: Disk-State Mismatch");
    }
    else for(size_t i{0}; i < disk.getUsers().size(); ++i){
        if (disk.getUsers().at(i)->getLogs().size() != this->users.at(i)->getLogs().size()){
            DiskHelper::printErr("State: Disk-State Mismatch");
        }
    }
    for (size_t i{0}; i < disk.getUsers().size(); ++i){
        auto logs {disk.getUsers().at(i)->getLogs()};
        for (size_t j{0}; j < logs.size(); ++j){
            if (disk.getUsers().at(i)->getLogs().at(j)->getParticipants().size() != 
                    this->users.at(i)->getLogs().at(j)->getParticipants().size()) {
                DiskHelper::printErr("State: Disk-State Mismatch");
            }
        }
    }


	std::vector<std::string> buffer;
	for (auto vec : _users)
		buffer.insert(buffer.end(), vec.begin(), vec.end());
	for (auto vec : _logs)
		buffer.insert(buffer.end(), vec.begin(), vec.end());
	for (auto vec : _parts)
		buffer.insert(buffer.end(), vec.begin(), vec.end());

	disk.writeToDisk(buffer);
}

State::State(std::string path) : disk(path) {
    while (true) {
        try {
            loadSave(path);
            break;
        } catch (DiskAccessError& e) {
            while(true){
                DiskHelper::printErr(e.what());
                std::cout << "Enter the path to a valid save file: ";
                if (std::getline(std::cin, path)){
                    break;
                }
            }
        }
    }
}

State::~State(){
    while(true){
        try {
            save();
        } catch (DiskAccessError& e){
            std::cout << "Enter the path to a valid save file: ";
            std::string path;
            std::getline(std::cin, path);
            disk.setFilePath(path);
        } catch (std::exception& e){
            DiskHelper::printErr(e.what());
        }
        break;
    }
}

void State::createUser(std::string username, std::string password){
    if (isUniqueUsername(username)){
        User* u = new User(username, password);
        addUser(u);
        disk.addUser(u);
    }
    else throw std::runtime_error{"Username is taken"};
}

bool State::logIn(std::string username, std::string password){
    bool authenticated {false};

    for (auto user : getUsers()) {
        if (username == user->getName()){
            if (password == user->getPasswd()){
                authenticated = true;
                setCurrentUser(user);
                break;
            }
            throw std::runtime_error {"Incorrect Password"};
        }
    }
    if (!authenticated) throw std::runtime_error {"User does not exist"};
    else return authenticated;
}

bool State::isUniqueUsername(std::string username){
    bool isUniqueName {true};
    for (auto user : users){
        if (username == user->getName()){
            isUniqueName = false;
        }
    }
    return isUniqueName;
}