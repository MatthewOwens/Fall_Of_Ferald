#ifndef FILEMANAGER_H
#define FILEMANAGER_H
#include <json/json.h>
#include <string>
#include <vector>
#include "Node.h"

// Class to parse json dialogue modules and flag files.
class FileManager
{
public:
	FileManager();
	~FileManager();
	void loadDialogue(const std::string& moduleFile);
	std::vector<Node*> loadDialogue(const std::string& moduleFile, std::string& moduleName);
	bool saveDialogue(const std::string& moduleFile, const std::string& moduleName, std::vector<Node*>);
private:
	const std::string globalFlagPath = "dialogue/globalFlags.json";
	const std::string dialogueFolder = "dialogue/";
};

#endif//FILEMANAGER_H