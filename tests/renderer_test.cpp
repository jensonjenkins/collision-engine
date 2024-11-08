#include "../src/renderer/renderer.hpp"
#include "SFML/Graphics/Color.hpp"
#include "SFML/System/Clock.hpp"
#include <cstdint>
#include <iostream>
#include <sstream>

constexpr uint32_t fps_cap = 60;

void basic_integration_with_solver() {    
    environment<vec2<float32_t>> env(vec2<float32_t>{300.f, 300.f});
    renderer<vec2<float32_t>> r(env);

    sf::Font font;
    font.loadFromFile("assets/tuffy.ttf");

    sf::Text fps_text;
    fps_text.setFont(font);
    fps_text.setCharacterSize(12);
    fps_text.setFillColor(sf::Color::White);
    fps_text.setPosition(10.f, 10.f);

    r.set_frame_limit(fps_cap);

    for (uint32_t i = 0; i < 1200; ++i) {
        auto *p = new particle<vec2<float32_t>>();
        p->radius = 3;
        p->position = vec2<float32_t>(30 + i * 0.1, 60 + i * 0.1);  
        p->prev_position = vec2<float32_t>(p->position.i() - 0.05f, p->position.j());
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
        std::ostringstream ss;
        ss<<"FPS: "<<fps;
        fps_text.setString(ss.str());
 
        r.render_with_fps(fps_text);
    }
}

int main() {
    std::cout<<"Running renderer_test.cpp..."<<std::endl;
    
    basic_integration_with_solver();

    std::cout<<"renderer_test - ok."<<std::endl;

    return 0;
}
