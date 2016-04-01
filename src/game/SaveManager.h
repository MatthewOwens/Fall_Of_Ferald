/* The SaveManager class loads and saves game data to the save folder on disc */
#ifndef SAVEMANAGER_H
#define SAVEMANAGER_H
#include <list>
#include <string>
#include "Unit.h"
class SaveManager
{
	public:
		int save(const int saveSlot);
		int combatSave(const std::list<Unit>& playerUnits, const std::list<Unit>& computerUnits);

		int load(const int saveSlot);
		int combatLoad(std::list<Unit>& playerUnits, std::list<Unit>& computerUnits);
		SaveManager() {jsonTest();}
	private:
		std::string getFolderPath(const int saveSlot);
		bool folderExists(const std::string & path);
		int createFolderPath(const std::string& path);

		int saveCombatUnits(std::list<Unit>& unitList, std::string& filepath);
		int loadCombatUnits(std::ifstream& file, std::list<Unit>& unitList);
		void jsonTest();

		const std::string saveFolder = "saves/";

};
#endif //SAVEMANAGER_H
