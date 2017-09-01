#include "ImageManager.h"
#include <fstream>
#include <iostream>

ImageManager::ImageManager()
{
    //ctor
}

ImageManager::~ImageManager()
{
    //dtor
}

// Load an image into the map and use the file path as the key
bool ImageManager::loadImage(const std::string& filename)
{
    sf::Texture tempTexture;

    // Ensuring that the file exists
    if (!tempTexture.loadFromFile(filename))
    {
        std::cerr << "Error loading " << filename << std::endl;
        return false;
    }

    images[filename] = tempTexture;
    return true;
}

// Load an image into the map and specify the key
bool ImageManager::loadImage(const std::string& filename, const std::string& name)
{
    sf::Texture tempTexture;

    // Ensuring that the file exists
    if (!tempTexture.loadFromFile(filename))
    {
        std::cerr << "Error loading " << filename << std::endl;
        return false;
    }

    images[name] = tempTexture;
    return true;
}

bool ImageManager::loadAnimation(const std::string& filename)
{
    //if(loadImage(filename)
    sf::Texture tempTexture;

    // Ensuring that the file exists
    if (!tempTexture.loadFromFile(filename))
    {
        images[filename] = tempTexture;
        Animation tempAnim(&images[filename]);
        animations[filename] = tempAnim;
        return true;
    }
       else return false;
}

bool ImageManager::loadAnimation(const std::string& filename, const std::string& name)
{
    if(loadImage(filename, name))
    {
        Animation tempAnim(&images[name]);
        animations[name] = tempAnim;
        return true;
    }
    else return false;
}

bool ImageManager::loadAnimation(const std::string& filename, int frameWidth,
                                 int frameHeight, int frameCount, int reel)
{
    if(loadImage(filename))
    {
        Animation tempAnim(&images[filename], frameWidth, frameHeight, frameCount, reel);
        animations[filename] = tempAnim;
        return true;
    }
       else return false;
}

bool ImageManager::loadAnimation(const std::string& filename, const std::string& name, int frameWidth,
                                 int frameHeight, int frameCount, int reel)
{
    if(loadImage(filename, name))
    {
        Animation tempAnim(&images[name], frameWidth, frameHeight, frameCount, reel);
        animations[name] = tempAnim;
        return true;
    }
       else return false;
}

// Method to unload an image from memory based on it's key
void ImageManager::unloadImage(const std::string& key)
{
    images.erase(key);
}

void ImageManager::unloadAnimation(const std::string& key)
{
    images.erase(key);
    animations.erase(key);
}

// Method to access a texture given its filename
sf::Texture& ImageManager::getTexture(const std::string& key)
{
    return images[key];
}

Animation& ImageManager::getAnimation(const std::string& key)
{
    return animations[key];
}
