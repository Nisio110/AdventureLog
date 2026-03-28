#include "../include/Disk.h"
#include "../include/constants.h"
#include <iostream>

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

		printErr("Key not found in KVL.");
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
		{std::cerr << "[ ERROR ] " << errMessage << std::endl;}

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
} // End of Namespace

// Constructors
Disk::Disk(){
	setFilePath("disk.yaml");
	readDiskContents(filePath);
}

// Read operations
void Disk::openDisk(std::string_view fp){ 
	diskFile.clear();
	diskFile.open(fp); 
	diskFile.seekg(0);

	if (!isDiskGood())
		printErr("Disk is not accessible");
	else return;
}

bool Disk::isDiskGood(){ 
	return (diskFile.good()); 
}

std::vector<std::string> Disk::readDiskContents(std::string_view fp){
	std::string line;
	if (!diskFile) {openDisk(fp);}
	diskContents.clear();

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
		printErr("No KV pairs found in attributes");
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
	for (size_t i{}; i < objects.size(); ++i){
		const short objLineNum {0};
		const KeyValueList obj {objects.at(i)};

		if (getAttrKey(obj, objLineNum) == keys::obj){
			if (getAttrValue(obj, objLineNum) == keys::user)
				{addUser(initUser(obj));}
			else if ((getAttrValue(obj, objLineNum) == keys::caveLog) || (getAttrValue(obj, objLineNum) == keys::hikeLog))
				{addLog(initLog(obj));}
			else if (getAttrValue(obj, objLineNum) == keys::participant) 
				{/*addParticipant(initParticipant(obj))*/;}
			else
				printErr("\\,;O;,/"); // error
		}
	}

}

void Disk::printUserDetails(){ // for testing
	for (auto u : users){
		std::cout << "= Printing user details ===\n"
				<< " ID: " << u->getID() << "\n"
				<< " Name: " << u->getName() << "\n"
				<< " Passwd: " << u->getPasswd() << "\n"
				<< "===========================\n"
				<< "\n";
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
	// TODO: Figure out how to store the log objects tied to a user.
	return u; 
}

Log* Disk::initLog(KeyValueList attrs){
	// I think I need to face reality and seperate this function into
	// initCaveLog and initHikeLog.

	const size_t objKeyLineNum {0};
	Log* log {nullptr};

	size_t i {0};
	std::string id				{getAttrValue(attrs,++i)};
	std::string ownerId			{getAttrValue(attrs,++i)};
	std::string date			{getAttrValue(attrs,++i)};
	std::string area			{getAttrValue(attrs,++i)};
	std::string durationMins	{getAttrValue(attrs,++i)};
	std::string note			{getAttrValue(attrs,++i)};

	
	if (getAttrValue(attrs, objKeyLineNum) == keys::caveLog){
		log = new CaveLog();
	}
	else if (getAttrValue(attrs, objKeyLineNum) == keys::hikeLog){
		log = new HikeLog();
	}
	if (log){
		log->setID(strToNum(id));
		log->setOwnerId(strToNum(ownerId));
		log->setDate(date);
		log->setArea(area);
		log->setDurationMins(strToNum(durationMins));
		log->setNote(note);
	}
	return log;
}

Disk::~Disk(){
	for (auto uPtr : users) {delete uPtr;}
	users.clear();
	for (auto lPtr : logs) {delete lPtr;}
	logs.clear();
	for (auto pPtr : participants) {delete pPtr;}
	participants.clear();
}