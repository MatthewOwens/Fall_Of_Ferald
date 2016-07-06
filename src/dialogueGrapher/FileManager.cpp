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

std::vector<Node*> FileManager::loadDialogue(const std::string& moduleFile, std::string& moduleName)
{
	std::ifstream ifs(dialogueFolder + moduleFile);
	Json::Reader reader;
	Json::Value obj;
	std::vector<Node*> finalNodes;

	if (ifs.good())
	{
		reader.parse(ifs, obj);
		const Json::Value& nodes = obj["nodes"];

		moduleName = obj["module"].asString();
		
		// Populating the initial nodes
		for (int i = 0; i < nodes.size(); ++i)
		{
			finalNodes.push_back(new Node(nodes[i]["id"].asString()));
			auto currentNode = finalNodes.back();

			currentNode->setHeader(nodes[i]["header"].asString());
			currentNode->setBody(nodes[i]["body"].asString());
		}

		// Now that all the nodes are created, populate the connections
		for (int i = 0; i < nodes.size(); ++i)
		{
			const Json::Value& conns = nodes[i]["connectors"];

			// Adding the connections
			for (int j = 0; j < conns.size(); ++j)
			{
				// Getting this connections flags
				const Json::Value& requiredFlags = conns[j]["requiredFlags"];
				const Json::Value& triggeredFlags = conns[j]["triggeredFlags"];

				Node* connectionTarget = NULL;
				const std::string& targetID = conns[j]["targetID"].asString();

				// Finding the pointer to our final node
				for (auto n : finalNodes)
				{
					if (targetID == n->getIdentifier())
					{
						connectionTarget = n;
						break;
					}
				}

				if (connectionTarget == NULL)
				{
					std::cerr << "Cannot create connection, node with ID " << targetID;
					std::cerr << " doesn't exist!" << std::endl;
					break;	// Skipping to the next connection
				}

				Connector connection(finalNodes[i],
									connectionTarget,
									conns[j]["choiceText"].asString(),
									conns[j]["priority"].asInt());

				// Populating the flags if need be
				for (int k = 0; k < requiredFlags.size(); ++k)
					connection.addFlag(requiredFlags[k]["key"].asString(), requiredFlags[k]["value"].asBool());

				for (int k = 0; k < triggeredFlags.size(); ++k)
					connection.addTrigger(triggeredFlags[k]["key"].asString(), requiredFlags[k]["value"].asBool());

				finalNodes[i]->addConnector(connection);
			}
		}
	}
	else std::cerr << moduleFile << " not found!" << std::endl;

	return finalNodes;
}
bool FileManager::saveDialogue(const std::string& moduleFile, const std::string& moduleName, std::vector<Node*> nodes)
{
	std::ofstream ofs(dialogueFolder + moduleFile);
	Json::StyledWriter writer;

	Json::Value obj;
	Json::Value nodeArr(Json::arrayValue);
	if (ofs.good())
	{
		obj["module"] = moduleName;

		// Populating the node array
		for (auto i : nodes)
		{
			Json::Value nodeValue(Json::objectValue);
			Json::Value connArr(Json::arrayValue);

			nodeValue["id"] = i->getIdentifier();
			nodeValue["header"] = i->getHeader();
			nodeValue["body"] = i->getBody();

			// Populating the node's connections array
			for (auto j : i->getConnections())
			{
				Json::Value connValue(Json::objectValue);
				Json::Value flagArr(Json::arrayValue);
				Json::Value triggeredArr(Json::arrayValue);

				connValue["targetID"] = j.getEnd()->getIdentifier();
				connValue["choiceText"] = j.getChoiceText();
				connValue["priority"] = j.getPriority();

				// Populating the connection's flag array
				for (auto k : j.getFlags())
				{
					Json::Value flagValue(Json::objectValue);
					flagValue["key"] = k.first;
					flagValue["value"] = k.second;

					flagArr.append(flagValue);
				}

				// Populating the connection's triggered flags array
				for (auto k : j.getTriggers())
				{
					Json::Value flagValue(Json::objectValue);
					flagValue["key"] = k.first;
					flagValue["value"] = k.second;
					triggeredArr.append(flagValue);
				}

				connValue["requiredFlags"] = flagArr;
				connValue["triggeredFlags"] = triggeredArr;
				connArr.append(connValue);
			}

			nodeValue["connectors"] = connArr;
			nodeArr.append(nodeValue);
		}
		obj["nodes"] = nodeArr;

		// Writing out to the file
		ofs << obj << std::endl;
		ofs.close();
		return true;
	}
	else
	{
		std::cerr << "Could not open " << dialogueFolder + moduleFile << " for writing!" << std::endl;
		return false;
	}
}

FileManager::~FileManager()
{
}