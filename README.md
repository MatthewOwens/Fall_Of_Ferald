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
* Bind camera to map
* Replace current dialogue parser with one that supports json output from
  DialogueGrapher
* Implement a way to navigate the directed diagraphs outputted by DialogueGrapher
* Rework inventory system
* Make some sprites that don't look awful

## TODO - DialogueGrapher
* Implement Connector inlets/outlets
* Implement Connector deletion
* Fix scaling
* Stop connectors from breaking on Node::setID
* Implement tab selection in NodeView
* Fix NodeView selection not clearing until escape is pressed
* Implement wxWidgets to handle file dialogues
* Implement exportion of modules to JSON with jsoncpp
* Fix NodeView scaling bug when nodeviews are created at different scales
