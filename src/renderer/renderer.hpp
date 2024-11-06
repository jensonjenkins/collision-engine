#pragma once

#include "../physics/solver.hpp"
#include "SFML/Graphics/CircleShape.hpp"
#include "SFML/Graphics/RenderWindow.hpp"
#include "SFML/Window/WindowStyle.hpp"
#include <SFML/Window/Event.hpp>

/**
 * @tparam T    numeric primitive type (e.g. float32_t, float64_t, etc.)
 * @tparam TV   vector wrapper defined in particle.hpp (e.g. vec2, vec3)
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
     * Populate the frame with particles
     */
    void init_frame() {
        const std::vector<particle<T, VT>*>& particles = _env.particles();

        for(particle<T, VT>* particle : particles) {
            sf::CircleShape circle(particle->radius);
            circle.setPosition(particle->position.i(), particle->position.j());
            _frame_particles.push_back(circle);
        }
    }

    /**
     * Update position of particles
     */
    void update_frame() {
        const std::vector<particle<T, VT>*>& particles = _env.particles();

        // this may be temporary, consider updating position immediately 
        // after obj.position is updated to preserve locality
        for (int i = 0; i < particles.size(); i++) { 
            const auto* obj = particles[i];
            _frame_particles[i].setPosition(obj->position.i(), obj->position.j());
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
        for (const auto& particle : _frame_particles) {
            _window.draw(particle);
        }
        _window.display();
    }
    
private:
    environment<T, VT>&             _env;
    std::vector<sf::CircleShape>    _frame_particles;
    sf::RenderWindow                _window;

};
