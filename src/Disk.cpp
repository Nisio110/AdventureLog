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

// Constructors
Disk::Disk(){
	setFilePath("../disk.yaml");
	openDisk();
	if (!isFileGood()) 
		cout << "[ERROR] Disk is not accessible\n";
	readFileContents();
}

// Read operations
void Disk::openDisk(){ 
	diskFile.clear();
	diskFile.open(filePath); 
}

bool Disk::isFileGood(){ return (diskFile.good()); }

void Disk::readFileContents(){
	std::string line;
	fileContents.clear();

	diskFile.clear(); 
	diskFile.seekg(0);

	if (!isFileGood()) 
		cout << "[ERROR] Disk is not accessible\n";
	else 
		while( std::getline(diskFile >> std::ws, line))
			fileContents.push_back(line);
	// we want to be able to access each line of the file easily.
}
std::pair<std::string, std::string> Disk::parseStr(size_t lineNum)
{
	const std::string delimiter {": "};
	std::string line {fileContents.at(lineNum)};
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

std::vector<std::pair<std::string, std::string>> Disk::parseFile()
{
	readFileContents();
	std::vector<std::pair<std::string, std::string>> parsedVals;
	for (size_t i{0}; i < fileContents.size(); ++i)
		parsedVals.push_back(parseStr(i));	
	return parsedVals;
}


void Disk::splitByObjects(){
	for (size_t i{0}; i < attributes.size();){
		if (std::get<keys::key>(attributes.at(i)) == keys::obj) {
			std::vector<std::pair<std::string,std::string>> objBody;
			while (std::get<keys::key>(attributes.at(i)) != keys::div){
				objBody.push_back(attributes.at(i++));
			}
			objects.push_back(objBody);
			++i;
		}
	}

	if (globals::verboseMode){
		std::cout << "\n=== Printing object bodies =========\n";
		for (auto objBody : objects){
			std::cout << "= start of object ===\n";
			for (auto attribute : objBody){
				std::cout << std::get<keys::key>(attribute) << ": " << std::get<keys::val>(attribute) << "\n";
			}
			std::cout << "= end of object ===\n\n";
		}
		std::cout << "=== Done printing object bodies =========\n";
	}
}

void Disk::initProgram()
{
	attributes = parseFile();
	splitByObjects();
	
	for (size_t i{}; i < objects.size(); ++i){
		const short objLineNum {0};
		const std::vector<std::pair<std::string,std::string>> obj{objects.at(i)};
		const std::pair<std::string,std::string> pair = obj.at(objLineNum);
		const std::string key {std::get<keys::key>(pair)};
		const std::string val {std::get<keys::val>(pair)};

		if (key == keys::obj){
			if (val == keys::user) addUser(initUser(obj));
			else if (val == keys::caveLog || val == keys::hikeLog); //addLog(initLog(obj));
			else if (val == keys::caveLog); //addParticipant(initParticipant(obj));
			else std::cout << "\\,;O;,/" << "\n";
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
std::string getAttrValue(std::vector<std::pair<std::string,std::string>> attrs, size_t i){
	return std::get<keys::val>(attrs.at(i));
}
unsigned long strToNum(const std::string& id){
	const short base {10};
	return std::stoul(id.c_str(),nullptr, base);
}

User* Disk::initUser(std::vector<std::pair<std::string,std::string>> attrs){
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

Log* initLog(std::vector<std::pair<std::string,std::string>> attrs){
	// I think I need to face reality and seperate this function into
	// initCaveLog and initHikeLog.

	std::string id			{getAttrValue(attrs,++i)};
	std::string ownerId		{getAttrValue(attrs,++i)};
	std::string date		{getAttrValue(attrs,++i)};
	std::string area		{getAttrValue(attrs,++i)};
	std::string durationMins{getAttrValue(attrs,++i)};
	std::string note		{getAttrValue(attrs,++i)};

	const size_t objKeyLineNum {0};
	size_t i{0};
	if (getAttrValue(attrs, objKeyLineNum) == keys::caveLog){
		CaveLog* log = new CaveLog();
		log->setID(strToNum(id));
		log->setOwnerId(strToNum(ownerId));
		log->setDate(date);
		log->setArea(area);
		log->setDurationMins(strToNum(durationMins));
		log->setNote(note);
	}
	else if (getAttrValue(attrs, objKeyLineNum) == keys::hikeLog){
		HikeLog* log = new HikeLog();
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