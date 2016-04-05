/*
 *	Helper class used by Node to represent a connection between
 *	multiple dialogue nodes.
 */

#ifndef CONNECTOR_H
#define CONNECTOR_H
#include <string>
#include <vector>
#include <map>
class Connector
{
	friend class Node;

	public:
		Connector(std::string nextNodeID, int priority = -1);
		Connector(std::string nextNodeID, std::string choiceText,
				  int priority = -1);
		~Connector();
		void addFlag(std::string flag, bool state);

	protected:
		bool conditionsMet(std::map<std::string, bool>& localFlags,
						   std::map<std::string, bool>& globalFlags);
	//private:
		std::string nextNodeID;
		std::string choiceText;
		std::map<std::string, bool> flags;
		int priority;	// priority values <0 are auto resolved

};
#endif//CONNECTOR_H
