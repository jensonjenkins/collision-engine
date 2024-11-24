#pragma once

#include "common/utils.hpp"
#include "common/constants.hpp"
#include "physics/solver.hpp"
#include "SFML/Window/WindowStyle.hpp"
#include <SFML/Window/Event.hpp>
#include <SFML/Graphics.hpp>
#include <arm_neon.h>

namespace collision_engine {

struct renderer_metadata {
    std::string fps, latency, count;
    sf::Text fps_text, latency_text, particle_count;
    sf::Font font;

    renderer_metadata() {
        font.loadFromFile("assets/tuffy.ttf");

        fps_text.setFont(font);
        fps_text.setCharacterSize(12);
        fps_text.setFillColor(sf::Color::White);
        fps_text.setPosition(610.f, 10.f);

        latency_text.setFont(font);
        latency_text.setCharacterSize(12);
        latency_text.setFillColor(sf::Color::White);
        latency_text.setPosition(610.f, 22.f);

        particle_count.setFont(font);
        particle_count.setCharacterSize(12);
        particle_count.setFillColor(sf::Color::White);
        particle_count.setPosition(610.f, 36.f); 
    }
};

/**
 * @tparam VT vector wrapper defined in particle.hpp (e.g. vec2, vec3)
 */
template <typename VT, typename W>
class renderer {
private:
    using T = typename vec_traits<VT>::element_type;
public:
    explicit renderer(environment<VT, W>& env) 
        :   _env(env), 
            _r_metadata(),
            _window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Collision Engine", sf::Style::Close | sf::Style::Titlebar) {}
    
    /**
     * Populate the frame with particles on viewport
     */
    void init_frame() {
        const std::vector<particle<VT>*>& particles = _env.particles();

        for(particle<VT>* particle : particles) {
            sf::CircleShape circle(particle->radius);
            circle.setPosition(particle->position.i(), particle->position.j());
            circle.setFillColor(collision_engine::hsv_to_rgb(_hue, 0.8f, 0.8f));
            _frame_particles.push_back(circle);
            _hue += 0.05;
            if (_hue > _particle_color_cycle) _hue -= _particle_color_cycle;
        }
    }

    void add_object_to_frame(particle<VT>* particle) {
        sf::CircleShape circle(particle->radius);
        circle.setPosition(particle->position.i(), particle->position.j());
        circle.setFillColor(collision_engine::hsv_to_rgb(_hue, 0.8f, 0.8f));
        _frame_particles.push_back(circle);
        _hue += 0.05;
        if (_hue > _particle_color_cycle) _hue -= _particle_color_cycle;
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

    void set_frame_limit(uint32_t frame_limit) noexcept { _window.setFramerateLimit(frame_limit); }
   
    bool is_running() const noexcept { return _window.isOpen(); }

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
    
    void update_metadata(float32_t fps, float32_t latency, uint32_t count) { 
        _r_metadata.fps_text.setString("fps: " + std::to_string(fps));
        _r_metadata.latency_text.setString("latency: " + std::to_string(latency) + "s");
        _r_metadata.particle_count.setString("count: " + std::to_string(count));
    }

    void render_with_metadata() {
        // this may be temporary, consider updating position immediately 
        // after obj.position is updated to preserve locality
        _window.clear();

        for (const auto& particle : _frame_particles) {
            _window.draw(particle);
        }

        _window.draw(_r_metadata.fps_text);
        _window.draw(_r_metadata.latency_text);
        _window.draw(_r_metadata.particle_count);

        _window.display();
    }
    
private:
    float32_t                       _hue = 0;
    static constexpr float32_t      _particle_color_cycle = 360;
    environment<VT, W>&             _env;
    std::vector<sf::CircleShape>    _frame_particles;
    sf::RenderWindow                _window;
    renderer_metadata               _r_metadata;
};

} // namespace collision engine
