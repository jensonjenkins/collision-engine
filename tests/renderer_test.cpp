#include "../src/renderer/renderer.hpp"
#include <iostream>

void basic_integration_with_solver() {

    environment<vec2<float32_t>> env;
    auto *c1 = new particle<vec2<float32_t>>();

    c1->mass = 2;
    c1->radius = 10;
    c1->position = vec2<float32_t>(100, 100);
    c1->prev_position= vec2<float32_t>(99.8, 100.05);
    
    env.add_particle(c1);    

    renderer<vec2<float32_t>> r(env);

    r.init_frame();
    while(r.run()) {
        env.step(0.0018);
        r.update_frame();
        r.render();
    }
}

int main() {
    std::cout<<"Running renderer_test.cpp..."<<std::endl;
    
    basic_integration_with_solver();

    std::cout<<"renderer_test - ok."<<std::endl;

    return 0;
}
