#include "SaveManager.h"
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include "json/json.h"

#if defined(_WIN32)
	#include <direct.h> // for _mkdir
#endif

int SaveManager::save(const int saveSlot)
{

}

int SaveManager::combatSave(const std::list<Unit>& playerUnits,
		const std::list<Unit>& computerUnits)
{
	std::string folderPath = saveFolder + "combatSave/";

	std::string playerPath = folderPath + "playerUnits";
	std::string computerPath = folderPath + "computerUnits";
	std::ifstream playerFile;
	std::ifstream computerFile;

	//TODO: Storage
}

int SaveManager::load(const int saveSlot)
{

}

int SaveManager::combatLoad(std::list<Unit>& playerUnits, std::list<Unit>& computerUnits)
{
	std::string folderPath = saveFolder + "combatSave/";

	std::string playerPath = folderPath + "playerUnits";
	std::string computerPath = folderPath + "computerUnits";
	std::ifstream playerFile;
	std::ifstream computerFile;

	playerFile.open(playerPath);
	computerFile.open(computerPath);

	if(playerFile.good() != 0)
		return -1;
	if(computerFile.good() != 0)
		return -1;

	int ret = loadCombatUnits(playerFile, playerUnits);
	if(ret != 0)
	{
		std::cout << "loadCombatUnits() failed with return " << ret << std::endl;
		return ret;
	}
	else
	{
		ret = loadCombatUnits(computerFile, computerUnits);
		if(ret != 0)
			std::cout << "loadCombatUnits() failed with return " << ret << std::endl;
		return ret;

	}

}

std::string SaveManager::getFolderPath(const int saveSlot)
{
	std::stringstream sstream;

	sstream << saveFolder << saveSlot;
	return sstream.str();
}

int SaveManager::createFolderPath(const std::string& path)
{
	#if defined(_WIN32)
		int ret = _mkdir(path.c_str());
	#else
		mode_t mode = 0755;
		int ret = mkdir(path.c_str(), mode);
	#endif

	//TODO: error handling for mkdir failures.
	return ret;
}

bool SaveManager::folderExists(const std::string& path)
{
	#if defined(_WIN32)
		struct _stat info;
		if(_stat(path.c_str(), &info) != 0)
			return false;
		return (info.st_mode & S_IFDIR) != 0;
	#else
		struct stat info;
		if(stat(path.c_str(), &info) != 0)
			return false;
		return (info.st_mode & S_IFDIR) != 0;
	#endif
}

int SaveManager::loadCombatUnits(std::ifstream& file, std::list<Unit>& unitList)
{
	
}

int SaveManager::saveCombatUnits(std::list<Unit>& unitList, std::string& filepath)
{
	//TODO: json saving
	Json::Value jsonList;
	jsonList.append("hello");
	jsonList.append("world");

	Json::StyledWriter jsonWriter;
	std::cout << jsonWriter.write(jsonList);
}
void SaveManager::jsonTest()
{
	//TODO: json saving
	Json::Value jsonList;
	jsonList.append("hello");
	jsonList.append("world");

	Json::StyledWriter jsonWriter;
	std::cout << jsonWriter.write(jsonList);
}
