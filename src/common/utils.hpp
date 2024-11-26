#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Font.hpp>
#include <string>

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

struct renderer_metadata {
    std::string fps, latency, count;
    sf::Text fps_text, latency_text, particle_count;
    sf::Font font;

    renderer_metadata() {
        font.loadFromFile("assets/tuffy.ttf");

        fps_text.setFont(font);
        fps_text.setCharacterSize(12);
        fps_text.setFillColor(sf::Color::White);
        fps_text.setPosition(520.f, 10.f);

        latency_text.setFont(font);
        latency_text.setCharacterSize(12);
        latency_text.setFillColor(sf::Color::White);
        latency_text.setPosition(520.f, 23.f);

        particle_count.setFont(font);
        particle_count.setCharacterSize(12);
        particle_count.setFillColor(sf::Color::White);
        particle_count.setPosition(520.f, 36.f); 
    }
};

} // namespace collision_engine


