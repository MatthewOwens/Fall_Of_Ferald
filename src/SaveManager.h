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
		int combatSave(std::list<Unit>& playerUnits, std::list<Unit>& computerUnits);

		int load(const int saveSlot);
		int combatLoad();
	private:
		std::string getFolderPath(const int saveSlot);
		bool folderExists(const std::string & path);
		int createFolderPath(const std::string& path);

		int saveCombatUnits(std::list<Unit>& unitList);
		int loadCombatUnits(std::ifstream& file);

		#if defined(_WIN32)
			static const char* saveFolder = "saves\\"
		#else
			static const char* saveFolder = "saves/"
		#endif

}
#endif //SAVEMANAGER_H
