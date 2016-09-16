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
	return 0;
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
	return 0;
}

int SaveManager::load(const int saveSlot)
{
	return 0;
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
	return 0;
}

int SaveManager::saveCombatUnits(std::list<Unit>& unitList, std::string& filepath)
{
	//TODO: json saving
	Json::Value jsonList;
	Json::StyledWriter jsonWriter;

	for(auto i : unitList)
	{
		for(int j = 0; j < i.getStatsSize(); j++)
		{
			//jsonList[i.getName()]["base stats"][i.statNames[j]] = i.getStat(i.statNames[j]).getBase();
			//jsonList[i.getName()]["base stats"][i.statNames[j]] = i.getStat(i.statNames[j]).base;
			//jsonList[i.getName()]["mod stats"][i.statNames[j]] = i.getStat(i.statNames[j]).modifier;
		}
	}

	std::cout << jsonWriter.write(jsonList) << std::endl;
	return 0;

}
void SaveManager::jsonTest()
{
	std::map<std::string, int> stringMap;
	stringMap["One"] = 1;
	stringMap["Two"] = 2;
	stringMap["Three"] = 3;

	//TODO: json saving
	Json::Value jsonList;
	for(auto i : stringMap)
	{
		jsonList["Unit"][i.first] = i.second;
	}

	stringMap["One"] = 10;
	jsonList["One"] = stringMap["One"];

	Json::StyledWriter jsonWriter;

	std::cout << "Styled Json:" << std::endl;
	std::cout << jsonWriter.write(jsonList);
}
