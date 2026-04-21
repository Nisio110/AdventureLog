#include "../include/State.h"
#include "UI.h"

void State::setUsers(std::vector<User*> u){
    users = u;
}

void State::loadSave(){
    disk.loadFromDisk();
    users = disk.getUsers();
}
void State::loadSave(std::string diskPath){
    disk.loadFromDisk(diskPath);
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
        auto user = users.at(i);
        if (user->getId() == u->getId()) {
            delete user;
            users.erase(users.begin() + i);
        }
    }
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
            if (disk.getUsers().at(i)->getLogs().at(j)->getParticipants().size() 
                != 
                this->users.at(i)->getLogs().at(j)->getParticipants().size());
            DiskHelper::printErr("State: Disk-State Mismatch");
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

State::State(){
    loadSave();
    ui(*this);
}

State::State(std::string diskPath){
    loadSave(diskPath);
    ui(*this);
}

State::~State(){
    save();
}