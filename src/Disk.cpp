#include "../include/Disk.h"
#include "../include/Participant.h"
#include <iostream>
#include <sstream>
#include <stdexcept>

using namespace DiskHelper;

namespace DiskHelper{
	std::string getKey(const KeyValue& kv)
		{return std::get<keys::key>(kv);}
	std::string getVal(const KeyValue& kv)
		{return std::get<keys::val>(kv);}
	std::string getAttrValue(const KeyValueList& attrs, size_t i)
		{return std::get<keys::val>(attrs.at(i));}
	std::string getAttrKey(const KeyValueList& attrs, size_t i)
		{return std::get<keys::key>(attrs.at(i));}

	size_t getKeyLocationInObj(const std::string_view key, const KeyValueList& objKVList)
	{
		for (size_t i{}; i < objKVList.size(); ++i)
			{if (getKey(objKVList.at(i)) == key) {return i;}}

		throw std::runtime_error{"getKeyLocationInObj: Key not found in KVL."};
	}
	unsigned long strToNum(const std::string& id)
	{
		const short base {10};
		return std::stoul(id.c_str(),nullptr, base);
	}

	void printl(std::string_view str)
		{std::cout << str << '\n';}

	void printErr(std::string_view errMessage)
		{std::cerr << "\033[31m[ ERROR ]\033[0m " << errMessage << std::endl;}

	void printObject(const KeyValueList& objKVList){
		for (const auto& kv : objKVList)
		{
			std::cout << std::get<keys::key>(kv) 
					<< ": " 
					<< std::get<keys::val>(kv) 
					<< '\n';
		}
	}

	void printObjectKVL(const ObjectList& objects){
		printl("=== Printing object KV Lists =========");
		for (const auto& kvPairs : objects){
			printl("= printing object ===");
			printObject(kvPairs);
			printl("= end of object ===\n");
		}
		printl("=== Done printing object bodies =========\n");
	}

	bool doesSubstrExist(std::string_view str, std::string_view queryStr){
		// Return true if substring is found
		return (str.find(queryStr) != std::string::npos);
	}

	KeyValue strToKVPair(std::string_view kvStr)
	{
		const std::string delimiter {": "};
		const std::string div {"---"};
		std::string key {};
		std::string val {};

		// Is this string a div?
		size_t delimPos {kvStr.find(delimiter)};
		if (!doesSubstrExist(kvStr, delimiter)) {
			if (kvStr.contains(div)) 
				{key = div;}
			return {key,val};
		}

		// If it's not a div, then its a KVP
		key = kvStr.substr(0, delimPos);
		val = kvStr.substr(delimPos + delimiter.length());
		return {key, val};
	}

	KeyValueList StrVecToKVL(const std::vector<std::string>& kvlAsStr) {
		KeyValueList kvl;
		for (size_t i{0}; i < kvlAsStr.size(); ++i)
		{
			std::string kvStr {kvlAsStr.at(i)};
			kvl.push_back(strToKVPair(kvStr));
		}

		return kvl;
	}

	bool strToBool(std::string_view str){
		if (str == "false") {return false;}
		else if (str == "true"){return true;}
		else {
			throw std::runtime_error{"strToBool: String does not convert to boolean"};
		}
	}

	std::string boolToStr(bool x){
		if (x) return "true";
		else return "false";
	}
} // End of Namespace

// Constructors
Disk::Disk(std::string path){
	setFilePath(path);
}

// Read operations
void Disk::openFile(std::string_view fp){ 
	diskFile.clear();
	diskFile.open(std::string{fp});
	diskFile.seekg(0);

	if (!isFileGood()){
		throw DiskAccessError{"openFile: Disk is not accessible"};
	}
	else return;
}

bool Disk::isFileGood(){ 
	return (diskFile.good()); 
}

std::vector<std::string> Disk::readFileContents(std::string_view fp){
	std::string line;
	diskContents.clear();
	if (!diskFile.is_open()) {openFile(fp);}
	else { diskFile.clear(); diskFile.seekg(0); }

		while( std::getline(diskFile >> std::ws, line))
			diskContents.push_back(line);
	// we want to be able to access each line of the file easily.
	return diskContents;
}

void Disk::parseDisk(std::string_view fp)
{
	diskContents = readFileContents(fp);
	attributes = StrVecToKVL(diskContents);
	objects = splitByObjects(attributes);
}

ObjectList Disk::splitByObjects(const KeyValueList& attributes)
{
	if (attributes.empty()){
		throw std::runtime_error{"splitByObjects: No KV pairs found in attributes"};
	}
	for (size_t i{0}; i < attributes.size(); ++i){
		if (getAttrKey(attributes, i) != keys::obj)
			{continue;}
		KeyValueList objKVL;
		for (;(i < attributes.size()) && (getAttrKey(attributes, i) != keys::div); ++i)
			{objKVL.push_back(attributes.at(i));}
		objects.push_back(objKVL);
	}
	return objects;
}

void Disk::loadFromDisk(std::string diskPath)
{
	setFilePath(diskPath);
	parseDisk(diskPath);

	loadUsers(
		objLineNum, 
		loadLogs(
			objLineNum, 
			loadParticipants(objLineNum)
		)
	);

	// Load current object counts and update Ids so new objects ids don't
	// conflict with old ones ids
	User::setNumUsers(updateMaxId(users));
	Log::setNumLogs(updateMaxId(logs));
	Participant::setNumParticipants(updateMaxId(participants));
}

size_t Disk::updateMaxId(std::vector<Participant*> participants){
	int maxPartId{0};
	for (auto* p : participants)
		if (p->getId() > maxPartId) maxPartId = p->getId();
	return maxPartId;
}

size_t Disk::updateMaxId(std::vector<Log*> logs){
	int maxLogId{0};
	for (auto* l : logs)
		if (l->getId() > maxLogId) maxLogId = l->getId();
	return maxLogId;
}

size_t Disk::updateMaxId(std::vector<User*> users){
	int maxUserId{0};
	for (auto* u : users)
		if (u->getId() > maxUserId) maxUserId = u->getId();
	return maxUserId;

}

std::vector<Participant*> Disk::loadParticipants(size_t objLineNum){
	for (size_t i{0}; i < objects.size(); ++i){
		const KeyValueList obj {objects.at(i)};

		if (getAttrKey(obj, objLineNum) == keys::obj){
			if (getAttrValue(obj, objLineNum) == keys::participant) {
				if (Participant* p = initParticipant(obj)) { addParticipant(p); }
			}
		}
	}
	return participants;
}

std::vector<Log*> Disk::loadLogs(size_t objLineNum, std::vector<Participant*> participants){
	for (size_t i{0}; i < objects.size(); ++i){
		const KeyValueList obj {objects.at(i)};

		if (getAttrKey(obj, objLineNum) == keys::obj){
			if ((getAttrValue(obj, objLineNum) == keys::caveLog) || (getAttrValue(obj, objLineNum) == keys::hikeLog))
				{addLog(initLog(obj));}
		}
	}
	return logs;
}

std::vector<User*> Disk::loadUsers(size_t objLineNum, std::vector<Log*> logs){
	for (size_t i{0}; i < objects.size(); ++i){
		const KeyValueList obj {objects.at(i)};

		if (getAttrKey(obj, objLineNum) == keys::obj){
			if (getAttrValue(obj, objLineNum) == keys::user)
				{addUser(initUser(obj));}
		}
	}
	return users;
}

void Disk::printUserDetails(){ // for testing
	for (auto u : users){
		u->print();
	}
}

void Disk::printLogDetails(){ // for testing
	for (auto l : logs){
		l->print();
	}
}

void Disk::printParticipantDetails(){ // for testing
	for (auto p : participants){
		p->print();
	}
}

void Disk::printAll(){
	for (auto u : users){
		u->printAll();
	}
}

User* Disk::initUser(KeyValueList attrs){
	User* u = new User();
	size_t i {0};
	std::string id 		{getAttrValue(attrs,++i)};
	std::string name 	{getAttrValue(attrs,++i)};
	std::string passwd 	{getAttrValue(attrs,++i)};

	u->setID	(strToNum(id));
	u->setName	(name);
	u->setPasswd(passwd);


	for (auto l : logs){
		if (l->getUserId() == u->getId()) { u->addLog(l); }
	}

	// TODO: Figure out how to store the log objects tied to a user.
	return u; 
}

Log* Disk::initLog(KeyValueList logKVL){
	// Constants
	const size_t objKeyLineNum {0};
	Log* log {nullptr};
	// Initialise attribute variables
	std::string _id;
	std::string _ownerId;
	std::string _date;
	std::string _area;
	std::string _durMins;
	std::string _note;

	// Assign the generic values to the attributes
	for (auto i : logKVL){
		if 	( (_id.empty()) && (getKey(i) == keys::id))
			{ _id = getVal(i); }
		else if ( (_ownerId.empty()) && (getKey(i) == keys::ownerId))
			{ _ownerId = getVal(i); }
		else if ( (_date.empty()) && (getKey(i) == keys::date))
			{ _date = getVal(i); }
		else if ( (_area.empty()) && (getKey(i) == keys::area))
			{ _area = getVal(i); }
		else if ( (_durMins.empty()) && (getKey(i) == keys::durMins))
			{ _durMins = getVal(i); }
		else if ( (_note.empty()) && (getKey(i) == keys::note))
			{ _note = getVal(i); }
	}

	
	// CaveLog
	if (getAttrValue(logKVL, objKeyLineNum) == keys::caveLog){
		CaveLog* caveLog = new CaveLog();
		
		// Initialise the attribute vars
		std::string cName;
		std::string rigging;
		std::string cl;
		std::string srtCave;

		// Assign values to the attribute vars
		for (auto i : logKVL){
			if 	( (cName.empty()) && (getKey(i) == keys::cName))
				{ cName = getVal(i); }
			else if ( (rigging.empty()) && (getKey(i) == keys::rigging))
				{ rigging = getVal(i); }
			else if ( (cl.empty()) && (getKey(i) == keys::cl))
				{ cl = getVal(i); }
			else if ( (srtCave.empty()) && (getKey(i) == keys::srtCave))
				{ srtCave = getVal(i); }
		}

		// Assign the attribute values to the class object
		caveLog->setName(cName);
		caveLog->setSRTCave(strToBool(srtCave));
		caveLog->setCaveLeader(strToBool(cl));
		caveLog->setRigger(strToBool(rigging));

		// Assign the child object to the return object
		log = caveLog;
	}

	// HikeLog
	else if (getAttrValue(logKVL, objKeyLineNum) == keys::hikeLog){
		HikeLog* hikeLog = new HikeLog();

		// Initialise the attribute vars
		std::string dist;
		std::string weather;

		// Assign values to the attribute vars
		for (auto i : logKVL){
			if 	( (dist.empty()) && (getKey(i) == keys::dist))
				{ dist = getVal(i); }
			else if ( (weather.empty()) && (getKey(i) == keys::weather))
				{ weather = getVal(i); }
		}

		// Assign the attribute values to the objects
		hikeLog->setDist(strToNum(dist));
		hikeLog->setWeather(weather);

		// Assign the child object to the return object
		log = hikeLog;
	}

	// Assign the generic attributes to the object
	if (log){
		log->setID(strToNum(_id));
		log->setUserId(strToNum(_ownerId));
		log->setDate(_date);
		log->setArea(_area);
		log->setDurationMins(strToNum(_durMins));
		log->setNote(_note);
	}


	// Find and assign associated participants
	for (auto p : participants){
		if (p->getLogId() == log->getId()) { log->addParticipant(p); }
	}

	// Return the object
	return log;
}

Participant* Disk::initParticipant(KeyValueList PartKVL){
	Participant* p {nullptr};
	// Initialise attribute vars
	std::string name;
	std::string id;
	std::string logId;

	for (auto i : PartKVL){
		if ((name.empty()) && (getKey(i) == keys::name))
			{ name = getVal(i); }
		else if ((id.empty()) && (getKey(i) == keys::id))
			{ id = getVal(i); }
		else if ((logId.empty()) && (getKey(i) == keys::pLogId))
			{ logId = getVal(i); }
	}
	if (id.empty() || logId.empty()) {
		std::cerr << "Disk::initParticipant: skipping malformed Participant block "
		          << "(name='" << name << "', id='" << id << "', log-id='" << logId << "')\n";
		return nullptr;
	}
	p = new Participant(name);
	p->setID(stoi(id));
	p->setLogID(stoi(logId));
	return p;
}

Disk::~Disk(){
	for (auto uPtr : users) {delete uPtr;}
	users.clear();
	logs.clear();
	participants.clear();
}

void Disk::removeUser(User* u){
	for (size_t i{0}; i < users.size(); ++i){
		auto user = users.at(i);
		if (user->getId() == u->getId()) {
			delete user;
			users.erase(users.begin() + i);
		}
	}
}

std::vector<std::string> Disk::userToStr(User* u){
	std::vector<std::string> vecStr;

	std::string obj{keys::obj};
	std::string id{keys::id};
	std::string name{keys::name};
	std::string passwd{keys::passwd};

	obj.append(keys::sep);
	id.append(keys::sep);
	name.append(keys::sep);
	passwd.append(keys::sep);

	obj.append(keys::user);
	id.append(intToStr(u->getId()));
	name.append(u->getName());
	passwd.append(u->getPasswd());

	vecStr.push_back(obj);
	vecStr.push_back(id);
	vecStr.push_back(name);
	vecStr.push_back(passwd);

	std::string div{keys::div};
	vecStr.push_back(div);
	return vecStr;
}

std::string DiskHelper::intToStr(int x){
	std::stringstream buf;
	std::string str;
	buf << x;
	getline(buf,str);
	return str;
}

std::vector<std::string> Disk::logToStr(Log* log){
	std::vector<std::string> vecStr;	
	std::string obj{keys::obj};
	std::string id{keys::id};
	std::string durMins{keys::durMins};
	std::string area{keys::area};
	std::string note{keys::note};
	std::string date{keys::date};
	std::string uid{keys::ownerId};

	obj.append(keys::sep);
	id.append(keys::sep);
	durMins.append(keys::sep);
	area.append(keys::sep);
	note.append(keys::sep);
	date.append(keys::sep);
	uid.append(keys::sep);

	id.append(intToStr(log->getId()));
	durMins.append(intToStr(log->getDurationMins()));
	area.append(log->getArea());
	note.append(log->getNote());
	date.append(log->getDate());
	uid.append(intToStr(log->getUserId()));


	if (auto cave = dynamic_cast<CaveLog*>(log)){
		std::string name{keys::cName};
		std::string isSrt{keys::srtCave};
		std::string wasCL{keys::cl};
		std::string didRigging{keys::rigging};

		name.append(keys::sep);
		isSrt.append(keys::sep);
		wasCL.append(keys::sep);
		didRigging.append(keys::sep);

		obj.append(keys::caveLog);
		name.append(cave->getName());
		isSrt.append(boolToStr(cave->isSRTCave()));
		wasCL.append(boolToStr(cave->wasCaveLeader()));
		didRigging.append(boolToStr(cave->wasRigger()));
		
		vecStr.push_back(obj);
		vecStr.push_back(id);
		vecStr.push_back(durMins);
		vecStr.push_back(area);
		vecStr.push_back(note);
		vecStr.push_back(date);
		vecStr.push_back(uid);
		vecStr.push_back(name);
		vecStr.push_back(isSrt);
		vecStr.push_back(wasCL);
		vecStr.push_back(didRigging);
	}
	else if (auto hike = dynamic_cast<HikeLog*>(log)){
		std::string distance{keys::dist};
		std::string weather{keys::weather};

		distance.append(keys::sep);
		weather.append(keys::sep);

		obj.append(keys::hikeLog);
		distance.append(intToStr(hike->getDist()));
		weather.append(hike->getWeather());

		vecStr.push_back(obj);
		vecStr.push_back(id);
		vecStr.push_back(durMins);
		vecStr.push_back(area);
		vecStr.push_back(note);
		vecStr.push_back(date);
		vecStr.push_back(uid);
		vecStr.push_back(distance);
		vecStr.push_back(weather);
	}
	std::string div{keys::div};
	vecStr.push_back(div);
	return vecStr;
}

std::vector<std::string> Disk::partToStr(Participant* p){
	std::vector<std::string> vecStr;
	std::string obj{keys::obj};
	std::string name{keys::name};
	std::string id{keys::id};
	std::string logId{keys::pLogId};

	obj.append(keys::sep);
	name.append(keys::sep);
	id.append(keys::sep);
	logId.append(keys::sep);

	obj.append(keys::participant);
	name.append(p->getName());
	id.append(intToStr(p->getId()));
	logId.append(intToStr(p->getLogId()));

	vecStr.push_back(obj);
	vecStr.push_back(name);
	vecStr.push_back(id);
	vecStr.push_back(logId);

	std::string div{keys::div};
	vecStr.push_back(div);
	return vecStr;
}

void DiskHelper::openDiskForWriting(std::ofstream& file, std::string path){
	file.open(path,std::ios::out | std::ios::trunc);
	if (!file.good()) throw DiskAccessError{"openDiskForWriting: Failed to open disk for writing"};
	else return;
}

void Disk::writeToDisk(std::vector<std::string> buffer){
	openDiskForWriting(writeFile, filePath);
	for (auto str : buffer) {
		writeFile << str << "\n";
	}
}
