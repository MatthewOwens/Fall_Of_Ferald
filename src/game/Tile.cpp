#include "Tile.h"
#include <string>
#include <list>
#include <iostream>

// Default Constructor
Tile::Tile()
{

}

Tile::Tile(int terrainType)
{
	setStats(terrainType);
}

Tile::Tile(int terrainType, sf::Texture& texture)
{
    // Setting the texture
    sprite.setTexture(texture);
    sf::IntRect rect;
    rect.width = 64;
    rect.height = 64;
    rect.top = 0;
    rect.left = terrainType * rect.width;
    sprite.setTextureRect(rect);

	setStats(terrainType);
}

void Tile::setSprite(sf::Texture& texture, sf::IntRect& textureRect)
{
	sprite.setTexture(texture);
	sprite.setTextureRect(textureRect);
}

Tile::Tile(int terrainType, sf::Texture& texture, sf::IntRect& textureRect)
{
	// Setting the texture
	sprite.setTexture(texture);
	sprite.setTextureRect(textureRect);

	setStats(terrainType);
}

// Used to determine what unit movement types cannot move onto this tile
void Tile::setStats(int terrainType)
{
    switch (terrainType)
    {
        case 0:
            tileType = "plain";
            terrainDef = 1;
            break;
        case 1:
       		tileType = "wall";
            terrainDef = 0;
            break;
        case 2:
            tileType = "mountain";
            terrainDef = 3;
            break;
        case 3:
            tileType = "forest";
            terrainDef = 2;
            break;
        case 4:
            tileType = "water";
            terrainDef = 0;
            break;
        default:
            std::cout << "Terrain type is invalid, ensure the id is 0-4." << std::endl;
            std::cout << "The current id is: " << terrainType << std::endl;
            break;
    }
}
