#ifndef AUTOTILER_H
#define AUTOTILER_H
#include "ImageManager.h"
#include "json/json.h"
#include "Tileset.h"
#include "Tile.h"
#include <iostream>
#include <string>
#include <map>

class AutoTiler
{
public:
	AutoTiler(ImageManager* imageManager, const std::string& folderPath);
	~AutoTiler();

	/**
	 * Attempts to initilise textures for a tile array
	 * Returns true/false based on success
	 */
	bool setTextures(Tile** tiles, int width, int height, const std::string& tilesetName);

	/**
	 * Attempts to initilise the texture of a single tile based on it's neighbours
	 */
	void setTexture(Tile* target, Tile* neighbours[4], const std::string& tilesetName);

	/**
	 * Attempts to load a tileset, returns true/false based on success
	 */
	bool loadTileset(const std::string& tilesetFolder);

private:
	std::map<std::string, Tileset*> tilesets;

	ImageManager* const imageManager;
	const std::string folderPath;
	const std::string nameSuffix = "_tileset";

	/**
	 * Helper function for checking neighbours against a rule
	 * returns true if the rule is hit, false otherwise
	*/
	bool ruleCheck(Tile* neightbours[4], Json::Value& type, std::string& dirs);
};
#endif//AUTOTILER_H