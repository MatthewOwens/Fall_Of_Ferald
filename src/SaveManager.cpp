#include "SaveManager.h"
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>

#if defined(_WIN32)
	#include <direct.h> // for _mkdir
#endif

int SaveManager::save(const int saveSlot)
{

}

int SaveManager::combatSave(const std::list<Unit>& playerUnits,
		const std::list<Unit>& computerUnits)
{
	#if defined(_WIN32)
		std::string folderPath = saveFolder + "combatSave\\";
	#else
		std::string folderPath = saveFolder + "combatSave/";
	#endif

	std::string playerPath = folderPath + "playerUnits";
	std::string computerPath = folderPath + "computerUnits";
	std::ifstream playerFile;
	std::ifstream computerFile;

	//TODO: Storage
}

int SaveManager::load(const int saveSlot)
{

}

int SaveManager::combatLoad()
{
	#if defined(_WIN32)
		std::string folderPath = saveFolder + "combatSave\\";
	#else
		std::string folderPath = saveFolder + "combatSave/";
	#endif

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

	int ret = loadCombatUnits(playerFile);
	if(ret != 0)
	{
		std::cout << "loadCombatUnits() failed with return " << ret << std::endl;
		return ret;
	}
	else
	{
		ret = loadCombatUnits(computerFile);
		if(ret != 0)
			std::cout << "loadCombatUnits() failed with return " << ret << std::endl;
		return ret;

	}

}

std::string SaveManaager::getFolderPath(const int saveSlot)
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
		int ret = mkdir(path.c_str());
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

int SaveManager::loadCombatUnits(std::ifstream& file)
{
}

int SaveManager::saveCombatUnits(std::list<Unit>& unitList)
{

}
