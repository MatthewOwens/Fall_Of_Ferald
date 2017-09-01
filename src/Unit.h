#ifndef UNIT_H
#define UNIT_H
#include "ImageManager.h"
#include "Inventory.h"
#include <string>
#include <list>
#include <stack>
//#include <SFML/Graphics/Sprite.hpp>
#include "AnimatedSprite.h"

class Unit
{
    public:
        Unit(std::string unitName, std::string unitType, int lvl, int health, int strength, int magic, int skill,
            int speed, int defense, int resistance, int luck, int moveRange, int _x, int _y);
        Unit(std::string unitType, int _x, int _y, int* statArray, int lvl);
        virtual ~Unit();
        void modifyStat(std::string statName, int mod);
        //inline void setSprite(sf::Texture& texture) { sprite.setTexture(texture); }
        inline void setAnimation(Animation& anim) { sprite.setAnimation(anim); }
        int getStat(std::string statName);
        std::string getHighestStat();
        int getMaxRange();
        inline int getX(){return x;}
        inline int getY(){return y;}
        inline sf::Vector2i getGridPos() { return sf::Vector2i(x, y); }
        inline AnimatedSprite& getSprite() { return sprite; }
        inline bool getMoved() { return moved; }
        inline void setMoved(bool val) { moved = val; }
        inline Inventory& getInventory() { return inventory; }

        inline std::string getName() { return name; }
        inline std::string getMovementType() { return movementType; }

        void setPosition(int newX, int newY, int tileSize);	
        void setPosition(sf::Vector2f newPos, int tileSize);

        void moveAlong(std::stack<sf::Vector2i>& path, const int& tileSize);

        bool useItem(std::string itemName, Unit* target);	// For targeting other units
        bool useItem(std::string itemName);					// For using on self

        std::string getInfo();

        const int getStatsSize() { return 9; }

        // Structure used to store unit statisitics and modifiers
        struct Stat
        {
            Stat(int _base)
            {
                base = _base;
                modifier = 0;
            }
            Stat()
            {
                base = 0;
                modifier = 0;
            }
            int getCurrent() { return base - modifier; }

            int base;           // The base stat
            int modifier;       // The modifier to be applied to the stat
        };

		static const std::string statNames[9];

    private:
        //AnimatedSprite sprite;
        AnimatedSprite sprite;
        Inventory inventory;
        std::string movementType;   // The unit's movement type. (Treads, flight etc.)
        std::string type;           // The unit's type (Tank, Mech, Infantry etc.)
        std::string name;           // The unit's name (if not specified, this becomes the unit's type)
        int level;
		bool moved;					// Whether or not the unit has been moved this turn

        // The unit's stats, stored in a map so that individual stats can be easily accessed.
        std::map<std::string, Stat> stats;
        int x, y;                   // The unit's x and y coords on the map
};

#endif // UNIT_H
