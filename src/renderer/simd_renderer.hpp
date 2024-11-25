#pragma once

#include "common/utils.hpp"
#include "physics/simd_grid.hpp"
#include "physics/simd_solver.hpp"
#include "SFML/Window/WindowStyle.hpp"
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics.hpp>
#include <arm_neon.h>

namespace collision_engine::simd {

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

class renderer {
private:
    float32_t                           _hue = 0;
    f32_solver&                         _solver;
    renderer_metadata                   _r_metadata;
    sf::RenderWindow                    _window;
    std::vector<sf::CircleShape>        _frame_particles;
    static constexpr float32_t          _particle_color_cycle = 360;
    particle_collection<f32_solver::T>& _pc;

public:
    explicit renderer(f32_solver& solver) 
        :   _solver(solver), 
            _r_metadata(),
            _pc(solver.pc()),
            _window(sf::VideoMode(1280, 720), "Collision Engine", sf::Style::Close | sf::Style::Titlebar) {}

    /**
     * Populate the frame with particles on viewport
     */
    void init_frame() {
        for(uint32_t i = 0; i < _pc.xs.size(); i++) {
            sf::CircleShape circle(_pc.rs[i]);
            circle.setPosition(_pc.xs[i], _pc.ys[i]);
            circle.setFillColor(hsv_to_rgb(_hue, 0.8f, 0.8f));
            _frame_particles.push_back(circle);
            _hue += 0.05;
            if (_hue > _particle_color_cycle) _hue -= _particle_color_cycle;
        }
    }

    void add_object_to_frame(particle<f32_solver::T> p) {
        sf::CircleShape circle(p.r);
        circle.setPosition(p.x, p.y);
        circle.setFillColor(hsv_to_rgb(_hue, 0.8f, 0.8f));
        _frame_particles.push_back(circle);
        _hue += 0.05;
        if (_hue > _particle_color_cycle) _hue -= _particle_color_cycle; 
    }

    void update_frame() {
        // this may be temporary, consider updating position immediately 
        // after obj.position is updated to preserve locality
        for(uint32_t i = 0; i < _pc.xs.size(); i++) {
            _frame_particles[i].setPosition(_pc.xs[i], _pc.ys[i]);
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
};

} // namespace collision engine simd
