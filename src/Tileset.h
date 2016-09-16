#ifndef TILESET_H
#define TILESET_H
#include <SFML/Graphics/Texture.hpp>
#include <json/json.h>

class Tileset
{
	friend class AutoTiler;
public:
	Tileset(const sf::Texture& texture, Json::Value descriptor);
	~Tileset();

private:
	sf::Texture texture;
	Json::Value descriptor;
};
#endif//TILESET_H