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
		Connector(Node* startNode, Node* endNode, int priority = -1);
		Connector(Node* startNode, Node* endNode, std::string choiceText,
				  int priority = -1);
		~Connector();
		void addFlag(std::string flag, bool state);
		inline Node* const getEnd() { return endNode; }
		inline const std::string& getChoiceText() { return choiceText; }
		inline int getPriority() { return priority; }

	protected:
		bool conditionsMet(std::map<std::string, bool>& localFlags,
						   std::map<std::string, bool>& globalFlags);
	//private:
		Node* startNode;
		Node* endNode;
		std::string choiceText;
		std::map<std::string, bool> flags;
		int priority;	// priority values <0 are auto resolved

};
#endif//CONNECTOR_H
