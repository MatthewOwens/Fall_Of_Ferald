#include "Animation.h"

Animation::Animation(const sf::Texture* texture)
    :texture(texture)
{
}


// Constructor for animations with a fixed frame size
Animation::Animation(const sf::Texture* texture,
    int frameWidth, int frameHeight, int frameCount, int reel)
    :texture(texture)
{
    //Calculating the frame height
    sf::IntRect rect;
    rect.width = frameWidth;
    rect.height = frameHeight;
    rect.left = 0;
    rect.top = reel * frameHeight;

    //Populating the inital frames
    for(int i = 0; i < frameCount; ++i)
    {
        addFrame(rect);
        rect.left += rect.width;
    }
}
