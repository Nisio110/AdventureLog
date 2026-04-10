#include "../include/State.h"

void State::setUsers(std::vector<User*> u){
    users = u;
}

void State::initProgram(){
    disk.initProgram();
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
        if (users.at(i)->getId() == u->getId()){
            users.erase(users.begin() + i);
            return;
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


	std::vector<std::string> buffer;
	for (auto vec : _users)
		buffer.insert(buffer.end(), vec.begin(), vec.end());
	for (auto vec : _logs)
		buffer.insert(buffer.end(), vec.begin(), vec.end());
	for (auto vec : _parts)
		buffer.insert(buffer.end(), vec.begin(), vec.end());

	disk.writeToDisk(buffer);
}