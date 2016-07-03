Fall Of Ferald
==============

## Overview
SRPG based on the SRPG_Engine repo that was developed prior.
Currently in the prototype stage.

## Dependencies
SFML 2.3.2
jsoncpp

## TODO - Game
* Bind camera to map
* Replace current dialogue parser with one that supports json output from
  DialogueGrapher
* Implement a way to navigate the directed diagraphs outputted by DialogueGrapher
* Rework inventory system
* Make some sprites that don't look awful

## TODO - DialogueGrapher
* Fix sf::Text positions when adding new flags to connectors
* Fix sf::Text positions being changed prematurely when adding flags to connectors
* Add triggered flags to JSON tree
* Rename flags array to requiredFlags in JSON tree
* Implement Connector deletion
* Implement module scale flags
* Implement global scale flags
* Implement wxWidgets to handle file dialogues
