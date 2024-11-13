#include "../src/renderer/renderer.hpp"
#include "SFML/Graphics/Color.hpp"
#include "SFML/System/Clock.hpp"
#include "common/allocator.hpp"
#include <cstdint>
#include <iostream>

namespace collision_engine {

void allocate_particle_before_simulation() { 
    constexpr uint32_t fps_cap      = 80;
    constexpr uint32_t n_particles  = 6000;
    constexpr uint32_t world_width  = 600;
    constexpr uint32_t world_height = 600;

    using W = vec2<uint32_t>;
    using VT = vec2<float32_t>;
    using PT = particle<VT>;

    environment<VT, W> env(W{world_width, world_height});
    renderer<VT, W> r(env);

    sf::Font font;
    font.loadFromFile("assets/tuffy.ttf");

    sf::Text fps_text;
    fps_text.setFont(font);
    fps_text.setCharacterSize(12);
    fps_text.setFillColor(sf::Color::White);
    fps_text.setPosition(world_width + 10.f, 10.f);

    sf::Text latency_text;
    latency_text.setFont(font);
    latency_text.setCharacterSize(12);
    latency_text.setFillColor(sf::Color::White);
    latency_text.setPosition(world_width + 10.f, 22.f);

    sf::Text particle_count;
    particle_count.setFont(font);
    particle_count.setCharacterSize(12);
    particle_count.setFillColor(sf::Color::White);
    particle_count.setPosition(world_width + 10.f, 36.f);

    r.set_frame_limit(fps_cap);

    const float dt = 1.f / static_cast<float32_t>(fps_cap);

    sf::Clock clock;
    float32_t fps = 0;

    linear_allocator alloc(n_particles * sizeof(PT));
    for(int i=0;i<n_particles;i++) {
        void* allocated_memory = alloc.allocate(sizeof(PT), alignof(PT));
        auto *p = new (allocated_memory) PT();
        p->radius = 2;
        p->position = VT(100 + i * 0.01, 100 + i * 0.01);  
        p->prev_position = VT(p->position.i(), p->position.j());
        env.add_particle(p);
    }
    r.init_frame();
    while(r.run()) {
        env.step(dt);
        r.update_frame();

        // calculate fps
        float32_t c_time = clock.restart().asSeconds();
        fps = 1.f / c_time;
        fps_text.setString("fps: " + std::to_string(fps));
        latency_text.setString("latency: " + std::to_string(c_time) + "s");
        particle_count.setString("count: " + std::to_string(n_particles));
 
        r.render_with_fps(fps_text, latency_text, particle_count);
    }
}

void allocate_particle_during_simulation() { 
    constexpr uint32_t fps_cap      = 80;
    constexpr uint32_t n_particles  = 20000;
    constexpr uint32_t world_width  = 600;
    constexpr uint32_t world_height = 600;

    using W = vec2<uint32_t>;
    using VT = vec2<float32_t>;
    using PT = particle<VT>;

    environment<VT, W> env(W{world_width, world_height});
    renderer<VT, W> r(env);

    sf::Font font;
    font.loadFromFile("assets/tuffy.ttf");

    sf::Text fps_text;
    fps_text.setFont(font);
    fps_text.setCharacterSize(12);
    fps_text.setFillColor(sf::Color::White);
    fps_text.setPosition(world_width + 10.f, 10.f);

    sf::Text latency_text;
    latency_text.setFont(font);
    latency_text.setCharacterSize(12);
    latency_text.setFillColor(sf::Color::White);
    latency_text.setPosition(world_width + 10.f, 22.f);

    sf::Text particle_count;
    particle_count.setFont(font);
    particle_count.setCharacterSize(12);
    particle_count.setFillColor(sf::Color::White);
    particle_count.setPosition(world_width + 10.f, 36.f);

    r.set_frame_limit(fps_cap);

    const float dt = 1.f / static_cast<float32_t>(fps_cap);

    sf::Clock clock;
    float32_t fps = 0;
    int count = 0;

    linear_allocator alloc(n_particles * sizeof(PT));
    // r.init_frame();
    while(r.run()) {
        if (count < n_particles) {
            void* allocated_memory = alloc.allocate(sizeof(PT), alignof(PT));
            auto *p = new (allocated_memory) PT();
            p->radius = 2;
            p->position = VT(0, 100 - 9 * (count % 3));  
            p->prev_position = VT(-1, 100 - 9 * (count % 3));
            env.add_particle(p);
            r.add_object_to_frame(p);
            count++;
        } 
        env.step(dt);
        r.update_frame();

        // calculate fps
        float32_t c_time = clock.restart().asSeconds();
        fps = 1.f / c_time;
        fps_text.setString("fps: " + std::to_string(fps));
        latency_text.setString("latency: " + std::to_string(c_time) + "s");
        particle_count.setString("count: " + std::to_string(count));
 
        r.render_with_fps(fps_text, latency_text, particle_count);
    }
}

}
int main() {
    std::cout<<"Running renderer_test.cpp..."<<std::endl;
    
    collision_engine::allocate_particle_before_simulation();
    // collision_engine::allocate_particle_during_simulation();

    std::cout<<"renderer_test - ok."<<std::endl;

    return 0;
}

