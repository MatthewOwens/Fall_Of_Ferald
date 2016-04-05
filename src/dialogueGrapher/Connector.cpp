#include "Connector.h"

Connector::Connector(std::string nextNodeID, int priority)
{
	this->nextNodeID = nextNodeID;
	this->priority = priority;
	choiceText = "";
}

Connector::Connector(std::string nextNodeID, std::string choiceText,
					 int priority)
{
	this->nextNodeID = nextNodeID;
	this->choiceText = choiceText;
	this->priority = priority;
}

void Connector::addFlag(std::string flag, bool state)
{
	flags.insert(std::pair<std::string,bool>(flag, state));
}

bool Connector::conditionsMet(std::map<std::string, bool>& localFlags,
							  std::map<std::string, bool>& globalFlags)
{
	std::vector<bool> flagsMet;

	// Checking all the connection's required flags
	for(auto&& flag : flags)
	{
		flagsMet.push_back(false);

		// Giving the flags local to the current module precedence
		for(auto&& local : localFlags)
		{
			if(flag.first == local.first)
			{
				// Checking if the flag was met locally
				flagsMet.back() = (flag.second == local.second);
				break;
			}
		}

		// If the flag wasn't met, check the global flags
		if(!flagsMet.back())
		{
			for(auto&& global : globalFlags)
			{
				if(flag.first == global.first)
				{
					// Checking if the flag was met globally
					flagsMet.back() = (flag.second == global.second);
					break;
				}
			}
		}
	}

	// If any of the conditions wern't met, return false
	for(auto condition : flagsMet)
	{
		if(!condition)
			return false;
	}

	return true;
}

Connector::~Connector()
{
}
