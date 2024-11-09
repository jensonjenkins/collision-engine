#include "../src/renderer/renderer.hpp"
#include "SFML/Graphics/Color.hpp"
#include "SFML/System/Clock.hpp"
#include "common/allocator.hpp"
#include <cstdint>
#include <iostream>

namespace collision_engine {

constexpr uint32_t fps_cap      = 60;
constexpr uint32_t n_particles  = 1300;

void integration_with_solver() { 
    using W = vec2<uint32_t>;
    using VT = vec2<float32_t>;
    using PT = particle<VT>;

    environment<VT, W> env(W{300, 300});
    renderer<VT, W> r(env);

    sf::Font font;
    font.loadFromFile("assets/tuffy.ttf");

    sf::Text fps_text;
    fps_text.setFont(font);
    fps_text.setCharacterSize(12);
    fps_text.setFillColor(sf::Color::White);
    fps_text.setPosition(10.f, 10.f);

    sf::Text latency_text;
    latency_text.setFont(font);
    latency_text.setCharacterSize(12);
    latency_text.setFillColor(sf::Color::White);
    latency_text.setPosition(10.f, 22.f);

    r.set_frame_limit(fps_cap);
    
    linear_allocator alloc(n_particles * sizeof(PT));
    for (uint32_t i = 0; i < n_particles; ++i) {
        void* allocated_memory = alloc.allocate(sizeof(PT), alignof(PT));
        auto *p = new (allocated_memory) PT();
        p->radius = 3;
        p->position = VT(30 + i * 0.1, 60 + i * 0.1);  
        p->prev_position = VT(p->position.i() - 0.05f, p->position.j());
        env.add_particle(p);
    }

    const float dt = 1.f / static_cast<float32_t>(fps_cap);

    sf::Clock clock;
    float32_t fps = 0;

    r.init_frame();
    while(r.run()) {
        env.step(dt);
        r.update_frame();
         
        // calculate fps
        float32_t c_time = clock.restart().asSeconds();
        fps = 1.f / c_time;
        fps_text.setString("fps: " + std::to_string(fps));
        latency_text.setString("latency: " + std::to_string(c_time) + "s");
 
        r.render_with_fps(fps_text, latency_text);
    }
}

}
int main() {
    std::cout<<"Running renderer_test.cpp..."<<std::endl;
    
    collision_engine::integration_with_solver();

    std::cout<<"renderer_test - ok."<<std::endl;

    return 0;
}

