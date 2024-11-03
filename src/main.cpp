#include "SFML/Window/Window.hpp"
#include "SFML/Window/VideoMode.hpp"
#include "SFML/Window/WindowStyle.hpp"
#include <SFML/Window.hpp>

int main() {
    constexpr uint32_t window_width  = 1280;
    constexpr uint32_t window_height = 720;
    sf::Window window(sf::VideoMode(window_width, window_height), "Collision Engine", sf::Style::Close | sf::Style::Titlebar );

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


