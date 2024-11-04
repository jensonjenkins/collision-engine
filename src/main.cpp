#include "SFML/Graphics/CircleShape.hpp"
#include "SFML/Graphics/RenderWindow.hpp"
#include "SFML/Window/Window.hpp"
#include "SFML/Window/VideoMode.hpp"
#include "SFML/Window/WindowStyle.hpp"
#include <SFML/Window.hpp>

int main() {
    constexpr uint32_t window_width  = 1280;
    constexpr uint32_t window_height = 720;
    sf::CircleShape c1, c2;
    sf::RenderWindow window(sf::VideoMode(window_width, window_height), "Collision Engine", sf::Style::Close | sf::Style::Titlebar );

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            switch (event.type) {
            case sf::Event::Closed:
                window.close();
                break;
            default:
                break;
            }
        }
        window.display();
    }
    return (0);
}


