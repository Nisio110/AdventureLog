#include "../include/Disk.h"
#include "../include/constants.h"
#include <iostream>

using std::cout;

namespace keys{
	constexpr short key {0};
	constexpr short val {1};
	// keys
	const std::string_view div {"---"};
	const std::string_view obj {"object"};
	const std::string_view id {"id"};
	const std::string_view name {"name"};
	const std::string_view passwd {"password"};
	const std::string_view ownerId {"owner-id"};
	const std::string_view date {"date"};
	const std::string_view area {"area"};
	const std::string_view durMins {"duration-mins"};
	const std::string_view note {"note"};
	const std::string_view cName{"cave-name"};
	const std::string_view rigging {"rigging"};
	const std::string_view cl {"cave-leader"};
	const std::string_view srtCave {"srt-cave"};
	const std::string_view dist {"distance"};
	const std::string_view weather {"weather"};

	// vals
	const std::string_view user {"User"};
	const std::string_view caveLog {"CaveLog"};
	const std::string_view hikeLog {"HikeLog"};
	const std::string_view participant {"Participant"};
}

// === Helper functions =======================
std::string getKey(KeyValue kv){
	return std::get<keys::key>(kv);
}
std::string getVal(KeyValue kv){
	return std::get<keys::val>(kv);
}

std::string getAttrValue(KeyValueList attrs, size_t i){
	return std::get<keys::val>(attrs.at(i));
}
std::string getAttrKey(KeyValueList attrs, size_t i){
	return std::get<keys::key>(attrs.at(i));
}
size_t getKeyLocationInObj(const std::string_view key, KeyValueList objKVList){
	for (size_t i{}; i < objKVList.size(); ++i)
		{if (getKey(objKVList.at(i)) == key) {return i;}}

	printErr("Key not found in KVL.");
	return 0;
}
unsigned long strToNum(const std::string& id){
	const short base {10};
	return std::stoul(id.c_str(),nullptr, base);
}

void printl(std::string_view str){
	std::cout << str << '\n';
}

void printErr(std::string_view errMessage){
	std::cerr << "[ ERROR ] " << errMessage << std::endl;
}

void printObject(KeyValueList objKVList){
	for (auto kv : objKVList)
		{
			std::cout << std::get<keys::key>(kv) 
					  << ": " 
					  << std::get<keys::val>(kv) 
					  << '\n';
		}
}

void printObjectKVL(ObjectList objects){
	printl("=== Printing object KV Lists =========");
	for (auto kvPairs : objects){
		printl("= printing object ===");
		printObject(kvPairs);
		printl("= end of object ===\n");
	}
	printl("=== Done printing object bodies =========\n");
}
//==============================================

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

void Disk::readDiskContents(std::string_view fp){
	std::string line;
	if (!diskFile) {openDisk(fp);}
	diskContents.clear();

		while( std::getline(diskFile >> std::ws, line))
			diskContents.push_back(line);
	// we want to be able to access each line of the file easily.
}

KeyValue Disk::StrToKVPair(size_t lineNum, std::string_view line)
{
	const std::string delimiter {": "};
	size_t delimPos = line.find(delimiter);
	std::string key;
	std::string val;
	std::string div {"---"};

	if (delimPos == std::string::npos) {
		if (line.contains(div)) {
			//std::cout << "Divider line reached\n";
			key = "---";
		}
		return {key,val};
	}

	key = line.substr(0, delimPos);
	val = line.substr(delimPos + delimiter.length());
	return {key, val};
}

KeyValueList Disk::parseKVL(std::vector<std::string> rawKVList)
{
	KeyValueList parsedKVL;
	for (size_t i{0}; i < rawKVList.size(); ++i)
		{parsedVals.push_back(StrToKVPair(i));}
	return parsedKVL;
}
KeyValueList Disk::parseDisk(std::vector<std::string> rawStrings){
	readDiskContents();
	attributes = parseKVList(rawStrings);
	splitByObjects();

}


void Disk::splitByObjects(){
	for (size_t i{0}; i < attributes.size();){
		if (getAttrKey(attributes, i) == keys::obj)
		{
			KeyValueList objBody;
			while (getAttrKey(attributes, i) != keys::div && i < attributes.size())
			{
				objBody.push_back(attributes.at(i++));
			}
			objects.push_back(objBody);
			++i;
		}
	}
	printObjectKVL(objects);
}

void Disk::initProgram()
{
	parseKVList();
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
	/*
	for (auto lPtr : logs) 	delete lPtr;
	logs.clear();
	for (auto pPtr : participants) delete pPtr;
	participants.clear;
	*/
}

/*
Log& initLog(std::vector<std::pair<std::string,std::string>> attr){
	Log* log = new Log();
	size_t i{0};
	log->setID(std::get<keys::val>(attrs.at(i++)));
	log->setOwn

}
*/


// Test Functions
/*
void IOTests::readFile(){
	string fileContents, ERR;
	Disk d;
	if (!d.isFileGood()) cout << "Disk is not accessible\n";
	else{
		while(std::getline(d.getDiskFile(),fileContents)){
			cout << fileContents << "\n";
		}
		cout << "EOF";
	}
}
*/