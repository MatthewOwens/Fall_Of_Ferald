Fall Of Ferald
==============

## Overview
SRPG based on the SRPG_Engine repo that was developed prior.
Currently in the prototype stage.

## Completed Features
* Rudimentary levels
* Tooltips
* Dialogue Boxes
* Units
* Super Basic AI
* 2D Camera

## TODO - Game
* Make some sprites that don't look awful
* Replace current dialogue parser with one that supports json output from
  DialogueGrapher
* Implement a way to navigate the directed diagraphs outputted by DialogueGrapher
* Rework inventory system
* Bind camera to map
* Realign camera on state change

## TODO - DialogueGrapher
* Stop connectors from breaking on Node::setID
* Implement Connector creation & deletion
* Fix NodeView scaling
* Fix NodeView selection not clearing until escape is pressed
* Implement wxWidgets to handle file dialogues
* Implement exportion of modules to JSON with jsoncpp
