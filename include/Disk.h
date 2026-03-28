#ifndef DISK_H
#define DISK_H
#include <vector>
#include <fstream>
#include "User.h"
class Log;
class Participant;

using KeyValue     = std::pair<std::string, std::string>;
using KeyValueList = std::vector<KeyValue>;
using ObjectList   = std::vector<KeyValueList>;

namespace DiskHelper {
	// Helper functions
	std::string getKey(KeyValue kv);
	std::string getVal(KeyValue kv);
	std::string getAttrValue(KeyValueList attrs, size_t i);
	std::string getAttrKey(KeyValueList attrs, size_t i);
	size_t getKeyLocationInObj(std::string_view key, KeyValueList objKVList);
	unsigned long strToNum(const std::string& id);
	void printl(std::string_view str);
	void printErr(std::string_view errMessage);
	void printObject(KeyValueList objKVList);
	void printObjectKVL(ObjectList objects);
	bool doesSubstrExist(std::string_view str, std::string_view queryStr);
	KeyValue strToKVPair(std::string_view kvStr);
	KeyValueList StrVecToKVL(std::vector<std::string> kvlAsStr);
}

class Disk{
protected:
	static inline std::string filePath;
	std::ifstream diskFile;
	std::vector<std::string> diskContents;
	KeyValueList attributes;
	ObjectList objects;

	std::vector<User*> users;
	std::vector<Log*> logs;
	std::vector<Participant*> participants;

	ObjectList splitByObjects(KeyValueList kvl);
public:
	// Constructors
	Disk();
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
	void openDisk(std::string_view fp);
	bool isDiskGood();
	std::vector<std::string> readDiskContents(std::string_view fp = filePath);

	// Parsing functions
	KeyValue parseStr(size_t lineNum); // returns key-value pair
	void parseDisk(std::string_view fp = filePath);

	// Init functions
	void initProgram();
	User* initUser(KeyValueList attr);
	Log* initLog(KeyValueList attr);
	Participant* initParticipant(KeyValueList attr);

	void printUserDetails(); // for testing
	void loadObject();
};

namespace keys{
	constexpr short key {0};
	constexpr short val {1};
	// keys
	inline constexpr std::string_view div {"---"};
	inline constexpr std::string_view obj {"object"};
	inline constexpr std::string_view id {"id"};
	inline constexpr std::string_view name {"name"};
	inline constexpr std::string_view passwd {"password"};
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