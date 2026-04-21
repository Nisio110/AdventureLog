#ifndef DISK_H
#define DISK_H
#include <vector>
#include <fstream>
#include "User.h"
class Log;
class Participant;

using KeyValue     = std::pair<std::string, std::string>;
using AttrKeyValue = std::pair<std::size_t,std::string>;
using KeyValueList = std::vector<KeyValue>;
using ObjectList   = std::vector<KeyValueList>;

namespace DiskHelper {
	// Helper functions
	std::string getKey(const KeyValue& kv);
	std::string getVal(const KeyValue& kv);
	std::string getAttrValue(const KeyValueList& attrs, size_t i);
	std::string getAttrKey(const KeyValueList& attrs, size_t i);
	size_t getKeyLocationInObj(std::string_view key, const KeyValueList& objKVList);
	unsigned long strToNum(const std::string& id);
	void printl(std::string_view str);
	void printErr(std::string_view errMessage);
	void printObject(const KeyValueList& objKVList);
	void printObjectKVL(const ObjectList& objects);
	bool doesSubstrExist(std::string_view str, std::string_view queryStr);
	KeyValue strToKVPair(std::string_view kvStr);
	KeyValueList StrVecToKVL(const std::vector<std::string>& kvlAsStr);
	bool strToBool(std::string_view str);
	std::string intToStr(int x);
	std::string boolToStr(bool x);
	void openDiskForWriting(std::ofstream& file, std::string path);
}

class Disk{
protected:
	static inline std::string filePath;
	const static inline std::string defaultDiskPath {"../disk.yaml"};
	std::ifstream diskFile;
	std::vector<std::string> diskContents;
	KeyValueList attributes;
	ObjectList objects;

	std::vector<User*> users;
	std::vector<Log*> logs;
	std::vector<Participant*> participants;

	std::vector<std::string> writeBuffer; 
	std::ofstream writeFile;

	ObjectList splitByObjects(const KeyValueList& kvl);
	const size_t objLineNum {0};
public:
	// Constructors
	Disk(std::string diskPath = defaultDiskPath);
	~Disk();

	// Getters 
	inline std::string getFilePath(){return filePath;}
	inline std::ifstream& getDiskFile(){return diskFile;}
	inline std::vector<User*>& getUsers(){return users;}
	inline std::vector<Log*>& getLogs(){return logs;}
	inline std::vector<Participant*>& getParticipants(){return participants;}

	// Setters
	inline void setFilePath(std::string fp){filePath = fp;}

	inline void addUser(User* u){users.push_back(u);}
	inline void addLog(Log* log){logs.push_back(log);}
	inline void addParticipant(Participant* p){participants.push_back(p);}

	// File operations
	void openFile(std::string_view fp);
	bool isFileGood();
	std::vector<std::string> readFileContents(std::string_view fp = filePath);

	// Parsing functions
	void parseDisk(std::string_view fp = defaultDiskPath);

	// Init functions
	User* initUser(KeyValueList attr);
	Log* initLog(KeyValueList attr);
	Participant* initParticipant(KeyValueList attr);

	// Load parsed strings into program memory
	// (Facilitates object creation)
	// Each object load function requires the last! 
	// 		Participants -> Logs -> Users
	std::vector<Participant*> loadParticipants(size_t objLineNum);
	std::vector<Log*> loadLogs(size_t objLineNum, std::vector<Participant*> participants);
	std::vector<User*> loadUsers(size_t objLineNum, std::vector<Log*> logs);
	void loadFromDisk(std::string diskPath = defaultDiskPath);

	// Update max id reached
	size_t updateMaxId(std::vector<Participant*>);
	size_t updateMaxId(std::vector<Log*>);
	size_t updateMaxId(std::vector<User*>);

	// Display
	void printUserDetails(); // for testing
	void printLogDetails(); // for testing
	void printParticipantDetails(); // for testing
	void printAll();

	// Writing
	std::vector<std::string> userToStr(User* u);
	std::vector<std::string> logToStr(Log* log);
	std::vector<std::string> partToStr(Participant* p);
	void writeToDisk(std::vector<std::string> buffer);
};

namespace keys{
	constexpr short key {0};
	constexpr short val {1};
	// keys
	inline constexpr std::string_view sep {": "};
	inline constexpr std::string_view div {"---"};
	inline constexpr std::string_view obj {"object"};
	inline constexpr std::string_view id {"id"};
	inline constexpr std::string_view name {"name"};
	inline constexpr std::string_view passwd {"password"};
	inline constexpr std::string_view pLogId {"log-id"};
	inline constexpr std::string_view pUid {"user-id"};
	inline constexpr std::string_view ownerId {"owner-id"};
	inline constexpr std::string_view date {"date"};
	inline constexpr std::string_view area {"area"};
	inline constexpr std::string_view durMins {"duration-mins"};
	inline constexpr std::string_view note {"note"};
	inline constexpr std::string_view cName{"cave-name"};
	inline constexpr std::string_view rigging {"rigging"};
	inline constexpr std::string_view cl {"cave-leader"};
	inline constexpr std::string_view srtCave {"srt-cave"};
	inline constexpr std::string_view dist {"distance"};
	inline constexpr std::string_view weather {"weather"};

	// vals
	inline constexpr std::string_view user {"User"};
	inline constexpr std::string_view caveLog {"CaveLog"};
	inline constexpr std::string_view hikeLog {"HikeLog"};
	inline constexpr std::string_view participant {"Participant"};
}

#endif