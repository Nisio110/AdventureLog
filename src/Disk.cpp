#include "../include/Disk.h"
#include "../include/constants.h"
#include <iostream>

using std::cout;

// Constructors
Disk::Disk(){
	setFilePath("./disk.yaml");
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

std::vector<size_t> Disk::parseStrLocs(std::string_view targetStr){
	std::vector<size_t> lineNums;
	readFileContents();
	for (size_t i{0}; i < fileContents.size(); ++i){
		if (fileContents[i].starts_with(targetStr) && fileContents[i].find(targetStr) != std::string::npos)
			lineNums.push_back(i);
	}
	return lineNums;
}

std::vector<size_t> Disk::parseAttrLocs(std::string_view targetAttr){
	// Goal: Discern the location that an attribute can be found on.
	std::string targetStr;
	std::string assign{": "};
	targetStr.append(targetAttr).append(assign);

	return parseStrLocs(targetStr);
}

std::vector<size_t> Disk::parseAttrLocs(std::string_view targetAttr, std::string_view targetValue){
	std::string targetStr;
	std::string assign{": "};
	targetStr.append(targetAttr).append(assign).append(targetValue);

	return parseStrLocs(targetStr);
}

size_t Disk::parseStrLoc(std::string_view targetStr, std::vector<std::string_view> searchTarget){
	// In contrast to parseStrLocs, this function assumes only one instance of the target string
	// is present in the search target, and hence only returns one size_t location variable.
	size_t lineNum{0};
	for (size_t i{0}; i < searchTarget.size(); ++i){
		if (searchTarget[i].starts_with(targetStr) && searchTarget[i].find(targetStr) != std::string::npos)
			lineNum = i;
	}
	return lineNum;
}

std::string Disk::parseAttrValue(std::string_view targetAttr, std::vector<std::string_view> objectBody){
	std::string assign{": "};
	std::string _targetAttr;
	_targetAttr.append(targetAttr).append(assign);
	
	size_t attrLoc {parseStrLoc(_targetAttr, objectBody)};
	return _targetAttr.substr(_targetAttr.size(), objectBody[attrLoc].size() - _targetAttr.size());
}

std::vector<std::string> Disk::parseObjectBody(std::string targetObj, int objID){
	std::vector<std::string> objBody;
	std::string attrObject {"object"};
	// stores locations for each object of that type
	std::vector<size_t> objLocs {parseAttrLocs(attrObject, targetObj)};
	// Delete ===
	for (auto i :  objLocs) std::cout << i << " ";
		std::cout << '\n';
	// Delete ---
	return objBody;
}

std::pair<std::string, std::string> Disk::parseStr(size_t lineNum)
{
	const std::string delimiter {": "};
	std::string line {fileContents.at(lineNum)};
	size_t delimPos = line.find(delimiter);
	std::string key;
	std::string val;

	if (delimPos == std::string::npos) {
		if (line.contains("---")) {
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
	std::vector<std::pair<std::string, std::string>> parsedVals;
	for (size_t i{0}; i < fileContents.size(); ++i)
		parsedVals.push_back(parseStr(i));	
	return parsedVals;
}




namespace keys{
	// keys
	constexpr short key {0};
	constexpr short val {1};
	constexpr std::string_view div {"---"};
	constexpr std::string_view obj {"object"};
	constexpr std::string_view id {"id"};
	constexpr std::string_view name {"name"};
	constexpr std::string_view passwd {"password"};
	constexpr std::string_view ownerId {"owner-id"};
	constexpr std::string_view date {"date"};
	constexpr std::string_view area {"area"};
	constexpr std::string_view durMins {"duration-mins"};
	constexpr std::string_view note {"note"};
	constexpr std::string_view cName{"cave-name"};
	constexpr std::string_view rigging {"rigging"};
	constexpr std::string_view cl {"cave-leader"};
	constexpr std::string_view srtCave {"srt-cave"};
	constexpr std::string_view dist {"distance"};
	constexpr std::string_view weather {"weather"};

	// vals
	constexpr std::string_view user {"User"};
	constexpr std::string_view caveLog {"CaveLog"};
	constexpr std::string_view hikeLog {"HikeLog"};
	constexpr std::string_view participant {"Participant"};
}

void Disk::splitByObjects(){
	for (size_t i{0}; i < attributes.size();){
		if (std::get<keys::key>(attributes.at(i)) == keys::obj)
		{
			std::vector<std::pair<std::string,std::string>> objBody;
			while (std::get<keys::key>(attributes.at(i)) != keys::div)
			{
				objBody.push_back(attributes.at(i));
				++i;
			}
			objects.push_back(objBody);
			++i;
		}
	}

	if (globals::verboseMode){
		std::cout << "\n=== Printing object bodies =========\n";
		for (auto objBody : objects){
			for (auto attribute : objBody){
				std::cout << std::get<keys::key>(attribute) << ": " << std::get<keys::val>(attribute) << "\n";
			}
			std::cout << "= end of object ===\n\n";
		}
		std::cout << "=== Done printing =========\n";
	}
}
void Disk::initProgram()
{
	parseFile();
	splitByObjects();
	
	for (size_t i{}; i < objects.size(); ++i){
		std::vector<std::pair<std::string, std::string>> obj {objects.at(i)};
		std::pair<std::string, std::string> pair = obj.at(i);
		std::string key {std::get<keys::key>(pair)};
		std::string val {std::get<keys::val>(pair)};

		if (key == keys::obj){
			if (val == keys::user)
				users.push_back(initUser(obj));
			else if (val == keys::caveLog || val == keys::hikeLog)
				logs.push_back(initLog(obj));
			else if (val == keys::caveLog)
				participants.push_back(initParticipants(obj));
			else std::cout << "\\,;O;,/"
		}
	}

}




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