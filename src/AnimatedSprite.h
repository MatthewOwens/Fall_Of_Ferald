#ifndef AnimatedSprite_h
#define AnimatedSprite_h
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Transformable.hpp>
#include <SFML/System/Vector2.hpp>

#include "Animation.h"

class AnimatedSprite : public sf::Drawable, public sf::Transformable
{
    public:
        AnimatedSprite(sf::Time frameTime = sf::seconds(0.2f), bool paused = false, bool looped = true);

        void update(sf::Time deltaTime);
        void setAnimation(const Animation& animation);
        inline void setFrameTime(sf::Time time) { frameTime = time; }
        void play(const Animation& animation);
        inline void play() { isPaused = false; }
        inline void pause() { isPaused = true; }
        void stop();
        inline void setLooped(bool looped) { looping  = looped; }
        void setColor(const sf::Color& color);
        inline const Animation* getAnimation() const { return animation; }
        sf::FloatRect getLocalBounds() const;
        sf::FloatRect getGlobalBounds() const;
        inline bool isLooped() { return looping; }
        inline bool isPlaying() { return !isPaused; }
        inline sf::Time getFrameTime() const { return frameTime; }
        void setFrame(std::size_t newFrame, bool resetTime = true);

    private:
        const Animation* animation;
        sf::Time frameTime;
        sf::Time currentTime;
        std::size_t currentFrame;
        bool looping;
        bool isPaused;
        const sf::Texture* texture;
        sf::Vertex vertices[4];

        virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};
#endif // AnimatedSprite_h

