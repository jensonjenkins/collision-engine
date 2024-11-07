#include "SFML/Graphics/Color.hpp"

namespace collision_engine {
   
static sf::Color hsv_to_rgb(float hue, float saturation, float value) {
    int h = static_cast<int>(hue / 60) % 6;
    float f = (hue / 60) - h;
    float p = value * (1 - saturation);
    float q = value * (1 - f * saturation);
    float t = value * (1 - (1 - f) * saturation);

    switch (h) {
        case 0: return sf::Color(value * 255, t * 255, p * 255);
        case 1: return sf::Color(q * 255, value * 255, p * 255);
        case 2: return sf::Color(p * 255, value * 255, t * 255);
        case 3: return sf::Color(p * 255, q * 255, value * 255);
        case 4: return sf::Color(t * 255, p * 255, value * 255);
        default: return sf::Color(value * 255, p * 255, q * 255);
    }
}

} // namespace collision_engine


