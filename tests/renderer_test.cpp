#include "../src/renderer/renderer.hpp"
#include <iostream>

void basic_integration_with_solver() {
    environment<float32_t, vec2<float32_t>> env;
    particle2d<float32_t> *c1 = new particle2d<float32_t>();

    c1->mass = 2;
    c1->radius = 10;
    c1->position = vec2<float32_t>(100, 100);
    c1->velocity = vec2<float32_t>(0, 0);
    c1->force = vec2<float32_t>(0, 0);
    
    env.add_object(c1);    

    renderer<float32_t, vec2<float32_t>> r(env);

    r.init_frame();
    while(r.run()) {
        env.step(0.005);
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
