#include "../src/renderer/renderer.hpp"
#include "SFML/System/Clock.hpp"
#include "common/allocator.hpp"
#include <cstdint>
#include <iostream>

namespace collision_engine {

void allocate_particle_before_simulation() { 
    constexpr uint32_t fps_cap      = 60;
    constexpr uint32_t n_particles  = 1400;
    constexpr uint32_t world_width  = 512;
    constexpr uint32_t world_height = 512;

    using W = vec2<uint32_t>;
    using VT = vec2<float32_t>;
    using PT = particle<VT>;

    environment<VT, W> env(W{world_width, world_height});
    renderer<VT, W> r(env);

    r.set_frame_limit(fps_cap);

    const float dt = 1.f / static_cast<float32_t>(fps_cap);

    sf::Clock clock;
    float32_t fps = 0;

    linear_allocator alloc(n_particles * sizeof(PT));
    for(int i{n_particles};i--;) {
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
 
        r.update_metadata(fps, c_time, n_particles); 
        r.render_with_metadata();
    }
}

void allocate_particle_during_simulation() { 
    constexpr uint32_t fps_cap      = 60;
    constexpr uint32_t n_particles  = 15000;
    constexpr uint32_t world_width  = 512;
    constexpr uint32_t world_height = 512;

    using W = vec2<uint32_t>;
    using VT = vec2<float32_t>;
    using PT = particle<VT>;

    environment<VT, W> env(W{world_width, world_height});
    renderer<VT, W> r(env);

    r.set_frame_limit(fps_cap);
    const float dt = 1.f / static_cast<float32_t>(fps_cap);

    sf::Clock clock;
    float32_t radius        = 2;
    float32_t start_i       = 10;
    float32_t prev_i        = 9.4;
    float32_t fps           = 0;
    int count               = 0;

    linear_allocator alloc(n_particles * sizeof(PT));
    // r.init_frame();
    while(r.run()) {
        if (count < n_particles) {
            for (int i{5};i--;) {
                void* allocated_memory = alloc.allocate(sizeof(PT), alignof(PT));
                auto *p = new (allocated_memory) PT();
                p->radius = radius;
                p->position = VT(start_i, 50 - 5 * i);  
                p->prev_position = VT(prev_i, 50 - 5 * i);
                env.add_particle(p);
                r.add_object_to_frame(p);
            }
            count += 5;
        } 

        env.step(dt);
        r.update_frame();

        // calculate fps
        float32_t c_time = clock.restart().asSeconds();
        fps = 1.f / c_time;

        r.update_metadata(fps, c_time, count); 
        r.render_with_metadata();
    }
}

}
int main() {
    std::cout<<"Running renderer_test.cpp..."<<std::endl;
    
    // collision_engine::allocate_particle_before_simulation();
    collision_engine::allocate_particle_during_simulation();

    std::cout<<"renderer_test - ok."<<std::endl;

    return 0;
}

