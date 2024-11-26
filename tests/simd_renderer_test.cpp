#include "physics/simd_solver.hpp"
#include "renderer/simd_renderer.hpp"
#include <iostream>

namespace collision_engine::simd {

void allocate_particle_before_simulation() {
    constexpr uint32_t fps_cap      = 60;
    constexpr uint32_t n_particles  = 15000;
    const float dt = 1.f / static_cast<float32_t>(fps_cap);
    
    f32_solver solver(dt); 
    renderer r(solver);
    r.set_frame_limit(fps_cap);

    sf::Clock clock;
    float32_t radius    = 2;
    float32_t start_i   = 10;
    float32_t prev_i    = 9.5;
    float32_t fps       = 0;
    int count           = 0;
    r.init_frame();
    while(r.run()) {
        if (count < n_particles) {
            for (int i{4};i--;) {
                particle<float32_t> p(start_i, 50 - 5 * i, prev_i, 50 - 5 * i, radius);
                solver.add_particle(p);
                r.add_object_to_frame(p); 
            }
            count += 4;
        }
        solver.step();
        r.update_frame();

        // calculate fps
        float32_t c_time = clock.restart().asSeconds();
        fps = 1.f / c_time;
 
        r.update_metadata(fps, c_time, count); 
        r.render_with_metadata();
    }  
}

} // namespace collision engine simd

int main() {
    std::cout<<"Running simd_renderer_test.cpp..."<<std::endl;
    
    collision_engine::simd::allocate_particle_before_simulation();

    std::cout<<"renderer_test - ok."<<std::endl;

    return 0;
}
