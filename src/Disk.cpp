#include "../include/Disk.h"
#include "../include/Participant.h"
#include "../include/constants.h"
#include <iostream>
#include <sstream>

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

		printErr("getKeyLocationInObj: Key not found in KVL.");
		return {};
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
			printErr("strToBool: String does not convert to boolean");
			return false;
		}
	}
} // End of Namespace

// Constructors
Disk::Disk(){
	setFilePath("disk.yaml");
	readDiskContents(filePath);
}

// Read operations
void Disk::openDisk(std::string_view fp){ 
	diskFile.clear();
	diskFile.open(std::string{fp});
	diskFile.seekg(0);

	if (!isDiskGood()){
		printErr("openDisk: Disk is not accessible");
		return;
	}
	else return;
}

bool Disk::isDiskGood(){ 
	return (diskFile.good()); 
}

std::vector<std::string> Disk::readDiskContents(std::string_view fp){
	std::string line;
	diskContents.clear();
	if (!diskFile.is_open()) {openDisk(fp);}
	else { diskFile.clear(); diskFile.seekg(0); }

		while( std::getline(diskFile >> std::ws, line))
			diskContents.push_back(line);
	// we want to be able to access each line of the file easily.
	return diskContents;
}

void Disk::parseDisk(std::string_view fp)
{
	diskContents = readDiskContents(fp);
	attributes = StrVecToKVL(diskContents);
	objects = splitByObjects(attributes);
}

ObjectList Disk::splitByObjects(const KeyValueList& attributes)
{
	if (attributes.empty()){
		printErr("splitByObjects: No KV pairs found in attributes");
		return objects;
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

void Disk::initProgram()
{
	parseDisk();
	const short objLineNum {0};

	// Pass 1: Load participants
	for (size_t i{0}; i < objects.size(); ++i){
		const KeyValueList obj {objects.at(i)};

		if (getAttrKey(obj, objLineNum) == keys::obj){
			if (getAttrValue(obj, objLineNum) == keys::participant) 
				{ addParticipant(initParticipant(obj)); }
		}
	}

	// Pass 2: Load logs
	for (size_t i{0}; i < objects.size(); ++i){
		const KeyValueList obj {objects.at(i)};

		if (getAttrKey(obj, objLineNum) == keys::obj){
			if ((getAttrValue(obj, objLineNum) == keys::caveLog) || (getAttrValue(obj, objLineNum) == keys::hikeLog))
				{addLog(initLog(obj));}
		}
	}
	
	// Pass 3: Load users
	for (size_t i{0}; i < objects.size(); ++i){
		const KeyValueList obj {objects.at(i)};

		if (getAttrKey(obj, objLineNum) == keys::obj){
			if (getAttrValue(obj, objLineNum) == keys::user)
				{addUser(initUser(obj));}
		}
	}
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
		else if ((logId.empty()) && (getKey(i) == keys::pLogID))
			{ logId = getVal(i); }
	}
	p = new Participant(name);
	p->setID(stoi(id));
	p->setLogID(stoi(logId));
	return p;
}

Disk::~Disk(){
	for (auto uPtr : users) {delete uPtr;}
	users.clear();
	for (auto lPtr : logs) {delete lPtr;}
	logs.clear();
	for (auto pPtr : participants) {delete pPtr;}
	participants.clear();
}

std::vector<std::string> Disk::userToStr(User* u){
	std::vector<std::string> vecStr;
	std::stringstream idBuf;

	std::string id;
	std::string name;
	std::string passwd;

	idBuf << u->getId();
	getline(idBuf, id);
	name = u->getName();
	passwd = u->getPasswd();

	vecStr.push_back(id);
	vecStr.push_back(name);
	vecStr.push_back(passwd);

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

	std::string id;
	std::string area;
	std::string note;
	std::string date;
	std::string uid;

	id = intToStr(log->getId());
	area = log->getArea();
	note = log->getNote();
	date = log->getDate();
	uid = log->getUserId();

	vecStr.push_back(id);
	vecStr.push_back(area);
	vecStr.push_back(note);
	vecStr.push_back(date);
	vecStr.push_back(uid);

	if (auto cave = dynamic_cast<CaveLog*>(log)){
		std::string name;
		std::string isSrt;
		std::string wasCL;
		std::string didRigging;

		name = cave->getName();
		isSrt = boolToStr(cave->isSRTCave());
		wasCL = boolToStr(cave->wasCaveLeader());
		didRigging = boolToStr(cave->wasRigger());
		
		vecStr.push_back(name);
		vecStr.push_back(isSrt);
		vecStr.push_back(wasCL);
		vecStr.push_back(didRigging);
	}
	else if (auto hike = dynamic_cast<HikeLog*>(log)){
		std::string distance;
		std::string weather;

		distance = intToStr(hike->getDist());
		weather = hike->getWeather();

		vecStr.push_back(distance);
		vecStr.push_back(weather);
	}
}

std::vector<std::string> Disk::partToStr(Participant* p){
	std::vector<std::string> vecStr;
	std::string name;
	std::string id;
	std::string logId;

	name = p->getName();
	id = intToStr(p->getId());
	logId = intToStr(p->getLogId());

	vecStr.push_back(name);
	vecStr.push_back(id);
	vecStr.push_back(logId);

	return vecStr;
}


std::vector<std::string> Disk::createBuffer(std::vector<std::string> vec1, std::vector<std::string> vec2, std::vector<std::string> vec3){
	std::vector<std::string> buffer;
	for (auto str : vec1){
		buffer.push_back(str);
	}
	for (auto str : vec2){
		buffer.push_back(str);
	}
	for (auto str : vec3){
		buffer.push_back(str);
	}
	return buffer;
}

void DiskHelper::openDiskForWriting(std::ofstream& file, std::string path){
	file.open(path,std::ios::trunc);
	if (!file.good()) printErr("openDiskForWriting: Failed to open disk for writing");
	else return;
}

void Disk::writeToDisk(std::vector<std::string> buffer){
	openDiskForWriting(writeFile, filePath);
	for (auto str : buffer) {
		writeFile << str;
	}
}