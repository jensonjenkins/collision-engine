#include "../src/renderer/renderer.hpp"
#include "SFML/System/Clock.hpp"
#include "common/allocator.hpp"
#include <cstdint>
#include <iostream>

namespace collision_engine {

void allocate_particle_during_simulation() { 
    constexpr uint32_t fps_cap      = 60;
    constexpr uint32_t n_particles  = 28000;
    constexpr uint32_t world_width  = 1028;
    constexpr uint32_t world_height = 1028;

    using W = vec2<uint32_t>;
    using VT = vec2<float32_t>;
    using PT = particle<VT>;

    environment<VT, W> env(W{world_width, world_height});
    renderer<VT, W> r(env);

    r.set_frame_limit(fps_cap);
    const float dt = 1.f / static_cast<float32_t>(fps_cap);

    sf::Clock clock;
    float32_t radius        = 2;
    float32_t start_i       = 5;
    float32_t prev_i        = 3.5;
    float32_t fps           = 0;
    int count               = 0;

    linear_allocator alloc(n_particles * sizeof(PT));

    while(r.run()) {
        if (count < n_particles) {
            for (int i{20};i--;) {
                void* allocated_memory = alloc.allocate(sizeof(PT), alignof(PT));
                auto *p = new (allocated_memory) PT();
                p->radius = radius;
                p->position = VT(start_i, 200 - 5 * i);  
                p->prev_position = VT(prev_i, 199 - 5 * i);
                env.add_particle(p);
                r.add_object_to_frame(p);
            }
            count += 20;
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

} // namespace collision engine


int main() {
    std::cout<<"Running renderer_test.cpp..."<<std::endl;
    
    collision_engine::allocate_particle_during_simulation();

    std::cout<<"renderer_test - ok."<<std::endl;

    return 0;
}

