#ifndef Animation_h
#define Animation_h
#include <SFML/Graphics.hpp>
#include <vector>
class Animation
{
    public:
        Animation() { texture = NULL; }
        Animation(const sf::Texture* texture);
        Animation(const sf::Texture* texture, int frameWidth,
                  int frameHeight, int frameCount, int reel = 0);

        inline void addFrame(sf::IntRect rect) { frames.push_back(rect); }
        inline void setSpriteSheet(const sf::Texture* texture) { this->texture = texture; }
        inline const sf::Texture* getSpriteSheet() const { return texture; }
        inline std::size_t getSize() const { return frames.size(); }
        inline const sf::IntRect& getFrame(std::size_t n) const { return frames[n]; }

    private:
        std::vector<sf::IntRect> frames;
        const sf::Texture* texture;
};
#endif //Animation_h

