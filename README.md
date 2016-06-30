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
* Fix text not showing in ConnectionEditor::InputBox
* Update NodeView::ConnectorTexts and Connector::Priorities on edits
* Implement Connector deletion
* Implement module scale flags
* Implement global scale flags
* Implement Connector flag editing
* Implement wxWidgets to handle file dialogues
