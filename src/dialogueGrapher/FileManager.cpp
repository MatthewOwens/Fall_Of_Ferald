#include "FileManager.h"
#include <iostream>
#include <fstream>

FileManager::FileManager()
{
	/*std::ifstream ifs(globalFlagPath);
	Json::Reader reader;
	Json::Value obj;
	reader.parse(ifs, obj);

	const Json::Value& globals = obj["globals"];
	for (int i = 0; i < globals.size(); ++i)
	{
		std::cout << globals[i]["key"].asString();
		std::cout << " " << globals[i]["value"].asBool();
		std::cout << std::endl;
	}*/
}

void FileManager::loadDialogue(const std::string& moduleFile)
{
	std::ifstream ifs(dialogueFolder + moduleFile);
	Json::Reader reader;
	Json::Value obj;

	if (ifs.good())
	{
		reader.parse(ifs, obj);
		const Json::Value& nodes = obj["nodes"];

		std::cout << "Module Name: " << obj["module"].asString() << std::endl;
		for (int i = 0; i < nodes.size(); ++i)
		{
			// Getting the connections
			const Json::Value& conns = nodes[i]["connectors"];

			std::cout << "ID: " << nodes[i]["id"].asString() << std::endl;
			std::cout << "header: " << nodes[i]["header"].asString() << std::endl;
			std::cout << "body: " << nodes[i]["body"].asString() << std::endl;
			std::cout << "connectors:\n{" << std::endl;

			for (int j = 0; j < conns.size(); ++j)
			{
				// Getting this connections flags
				const Json::Value& flags = conns[j]["flags"];

				std::cout << "\ttargetID: " << conns[j]["targetID"].asString() << std::endl;
				std::cout << "\tchoiceText: " << conns[j]["choiceText"].asString() << std::endl;
				std::cout << "\tpriority: " << conns[j]["priority"].asInt() << std::endl;
				std::cout << "\tflags:\n\t{" << std::endl;

				for (int k = 0; k < flags.size(); ++k)
				{
					std::cout << "\t\t" << flags[k]["key"].asString() << std::endl;
					std::cout << "\t\t" << flags[k]["value"].asBool() << std::endl;
				}
				std::cout << "\t}" << std::endl;
			}
			std::cout << "}" << std::endl;
		}
	}
	else std::cerr << moduleFile << " not found!" << std::endl;
}

FileManager::~FileManager()
{
}