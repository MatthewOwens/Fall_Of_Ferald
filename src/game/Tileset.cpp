#include "TileSet.h"

Tileset::Tileset(const sf::Texture& texture, Json::Value descriptor)
{
	this->texture = texture;
	this->descriptor = descriptor;
}

Tileset::~Tileset()
{
}