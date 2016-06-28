#ifndef FILEMANAGER_H
#define FILEMANAGER_H
#include <json/json.h>
#include <string>

// Class to parse json dialogue modules and flag files.
class FileManager
{
public:
	FileManager();
	~FileManager();
	void loadDialogue(const std::string& moduleFile);
private:
	const std::string globalFlagPath = "dialogue/globalFlags.json";
	const std::string dialogueFolder = "dialogue/";
};

#endif//FILEMANAGER_H