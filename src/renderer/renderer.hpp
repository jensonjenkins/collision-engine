#pragma once

#include "../physics/solver.hpp"
#include "SFML/Graphics/CircleShape.hpp"
#include "SFML/Graphics/RenderWindow.hpp"
#include "SFML/Window/WindowStyle.hpp"
#include <SFML/Window/Event.hpp>

/**
 * @tparam T    numeric primitive type (e.g. float32_t, float64_t, etc.)
 * @tparam TV   vector wrapper defined in object.hpp (e.g. vec2, vec3)
 */
template <typename T, typename VT>
class renderer {
public:
    static constexpr size_t window_width = 1280;
    static constexpr size_t window_height = 720;
    explicit renderer(environment<T, VT>& env) 
        : _env(env)
        , _window(sf::VideoMode(window_width, window_height), 
                "Collision Engine", sf::Style::Close | sf::Style::Titlebar) {}
    
    /**
     * Populate the frame with objects
     */
    void init_frame() {
        const std::vector<object<T, VT>*>& objects = _env.objects();

        for(object<T, VT>* object : objects) {
            const auto* obj = static_cast<const particle2d<T>*>(object);
            sf::CircleShape circle(obj->radius);
            circle.setPosition(obj->position.i(), obj->position.j());
            _frame_objects.push_back(circle);
        }
    }

    /**
     * Update position of objects
     */
    void update_frame() {
        const std::vector<object<T, VT>*>& objects = _env.objects();

        // this may be temporary, consider updating position immediately 
        // after obj.position is updated to preserve locality
        for (int i = 0; i < objects.size(); i++) { 
            const auto* obj = static_cast<const particle2d<T>*>(objects[i]);
            _frame_objects[i].setPosition(obj->position.i(), obj->position.j());
        }
    }
   
    bool is_running() {
        return _window.isOpen();
    }

    bool run() {
        sf::Event event;
        while(_window.pollEvent(event)){
            switch (event.type) {
            case sf::Event::Closed:
                _window.close();
                break;
            default:
                break;
            }
        }
        return is_running();
    }

    /**
     * Show image to the viewport
     */
    void render() {
        // this may be temporary, consider updating position immediately 
        // after obj.position is updated to preserve locality
        _window.clear();
        for (const auto& object : _frame_objects) {
            _window.draw(object);
        }
        _window.display();
    }
    
private:
    environment<T, VT>&             _env;
    std::vector<sf::CircleShape>    _frame_objects;
    sf::RenderWindow                _window;

};
