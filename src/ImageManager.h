#ifndef IMAGEMANAGER_H
#define IMAGEMANAGER_H
#include <SFML/Graphics.hpp>
#include "Animation.h"


class ImageManager
{
    public:
        ImageManager();
        virtual ~ImageManager();

        bool loadImage(const std::string& filename);
        bool loadImage(const std::string& filename, const std::string& name);

        bool loadAnimation(const std::string& filename);
        bool loadAnimation(const std::string& filename, int frameWidth, int frameHeight, int frameCount, int reel = 0);
        bool loadAnimation(const std::string& filename, const std::string& name);
        bool loadAnimation(const std::string& filename, const std::string& name,
                           int frameWidth, int frameHeight, int frameCount, int reel = 0);

        void unloadImage(const std::string& key);
        void unloadAnimation(const std::string& key);

        sf::Texture& getTexture(const std::string& key);
        Animation& getAnimation(const std::string& key);

    private:
        std::map<std::string, sf::Texture> images;
        std::map<std::string, Animation> animations;
};

#endif // IMAGEMANAGER_H
