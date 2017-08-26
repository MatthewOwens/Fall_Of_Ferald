#include "AnimatedSprite.h"

AnimatedSprite::AnimatedSprite(sf::Time frameTime, bool paused, bool looped) :
animation(NULL), currentFrame(0), isPaused(paused), looping(looped), texture(NULL)
{
}

void AnimatedSprite::setAnimation(const Animation& animation)
{
    this->animation = &animation;
    this->texture = this->animation->getSpriteSheet();
    currentFrame = 0;
    setFrame(currentFrame);
}

void AnimatedSprite::play(const Animation& animation)
{
    if(this->animation != &animation)
        setAnimation(animation);
    play();
}

void AnimatedSprite::setColor(const sf::Color& color)
{
    vertices[0].color = color;
    vertices[1].color = color;
    vertices[2].color = color;
    vertices[3].color = color;
}

sf::FloatRect AnimatedSprite::getLocalBounds() const
{
    sf::IntRect rect = animation->getFrame(currentFrame);

    float width = static_cast<float>(std::abs(rect.width));
    float height = static_cast<float>(std::abs(rect.height));

    return sf::FloatRect(0.f, 0.f, width, height);
}

sf::FloatRect AnimatedSprite::getGlobalBounds() const
{
    return getTransform().transformRect(getLocalBounds());
}

void AnimatedSprite::setFrame(std::size_t newFrame, bool resetTime)
{
    if(animation)
    {
        //Calculate new vertex positions and texture coords
        sf::IntRect rect = animation->getFrame(newFrame);

        vertices[0].position = sf::Vector2f(0.f, 0.f);
        vertices[1].position = sf::Vector2f(0.f, static_cast<float>(rect.height));
        vertices[2].position = sf::Vector2f(static_cast<float>(rect.width),
                                            static_cast<float>(rect.height));
	vertices[3].position = sf::Vector2f(static_cast<float>(rect.width), 0.f);

        float left = static_cast<float>(rect.left) + 0.0001f;
        float right = left + static_cast<float>(rect.width);
        float top = static_cast<float>(rect.top);
        float bottom = top + static_cast<float>(rect.height);

        vertices[0].texCoords = sf::Vector2f(left, top);
        vertices[1].texCoords = sf::Vector2f(left, bottom);
        vertices[2].texCoords = sf::Vector2f(right, bottom);
        vertices[3].texCoords = sf::Vector2f(right, top);
    }

    if(resetTime)
        currentTime = sf::Time::Zero;
}

void AnimatedSprite::update(sf::Time deltaTime)
{
    if(!isPaused && animation)
    {
        currentTime += deltaTime;

	// Need to advance to the next frame
        if(currentTime > frameTime)
        {
            // Reset time, but keep remainder
            currentTime = sf::microseconds(currentTime.asMicroseconds() & frameTime.asMicroseconds());

            // get next frame index
            if(currentFrame + 1 < animation->getSize())
                currentFrame++;
            else
            {
                // Animation ended
                currentFrame = 0;

                if(!looping)
                    isPaused = true;
            }
        }

        setFrame(currentFrame, false);
    }
}

void AnimatedSprite::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    if(animation && texture)
    {
        states.transform *= getTransform();
        states.texture = texture;
        target.draw(vertices, 4, sf::Quads, states);
    }
}
