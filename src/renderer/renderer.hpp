#pragma once

#include "common/utils.hpp"
#include "common/constants.hpp"
#include "physics/solver.hpp"
#include "SFML/Window/WindowStyle.hpp"
#include <SFML/Window/Event.hpp>
#include <SFML/Graphics.hpp>

namespace collision_engine {
/**
 * @tparam VT vector wrapper defined in particle.hpp (e.g. vec2, vec3)
 */
template <typename VT, typename W>
class renderer {
private:
    using T = typename vec_traits<VT>::element_type;
public:
    explicit renderer(environment<VT, W>& env) 
        : _env(env), _window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), 
                "Collision Engine", sf::Style::Close | sf::Style::Titlebar) {}
    
    /**
     * Populate the frame with particles on viewport
     */
    void init_frame() {
        float32_t hue = 0;
        const std::vector<particle<VT>*>& particles = _env.particles();

        for(particle<VT>* particle : particles) {
            sf::CircleShape circle(particle->radius);
            circle.setPosition(particle->position.i(), particle->position.j());
            circle.setFillColor(collision_engine::hsv_to_rgb(hue, 0.8f, 0.8f));
            _frame_particles.push_back(circle);
            hue += 360.f / particles.size();
            if (hue > 360) hue -= 360;
        }
    }

    /**
     * Update position of SFML particles on viewport
     */
    void update_frame() {
        const std::vector<particle<VT>*>& particles = _env.particles();

        // this may be temporary, consider updating position immediately 
        // after obj.position is updated to preserve locality
        for (int i = 0; i < particles.size(); i++) { 
            const auto* obj = particles[i];
            _frame_particles[i].setPosition(obj->position.i(), obj->position.j());
        }
    }

    void set_frame_limit(uint32_t frame_limit) {
        _window.setFramerateLimit(frame_limit);
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

    /**
     * Show image to viewport with fps information
     */
    void render_with_fps(const sf::Text& fps, const sf::Text& latency) {
        // this may be temporary, consider updating position immediately 
        // after obj.position is updated to preserve locality
        _window.clear();

        for (const auto& particle : _frame_particles) {
            _window.draw(particle);
        }
        _window.draw(fps);
        _window.draw(latency);
        _window.display();
    }
    
private:
    environment<VT, W>&                _env;
    std::vector<sf::CircleShape>    _frame_particles;
    sf::RenderWindow                _window;

};

} // namespace collision engine
