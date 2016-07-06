Fall Of Ferald
==============

## Overview
SRPG based on the SRPG_Engine repo that was developed prior.
Currently in the prototype stage.

## Dependencies
SFML 2.3.2
jsoncpp

##

## TODO - Game
* Bind camera to map
* Replace current dialogue parser with one that supports json output from
  DialogueGrapher
* Implement a way to navigate the directed diagraphs outputted by DialogueGrapher
* Rework inventory system
* Make some sprites that don't look awful

## TODO - DialogueGrapher
* Fix connector lines being removed incorrectly
* Fix connector texts not being cleared on node deletion
* Add warning when saving with an 'untitled' module
* Implement wxWidgets to handle file dialogues
