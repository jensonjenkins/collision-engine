#include "../src/renderer/renderer.hpp"
#include <iostream>

int main() {
    std::cout<<"Running renderer_test.cpp..."<<std::endl;

    environment<float32_t, vec2<float32_t>> env;
    particle2d<float32_t> *c1 = new particle2d<float32_t>();

    c1->mass = 2;
    c1->radius = 2;
    c1->position = vec2<float32_t>(0, 100);
    c1->velocity = vec2<float32_t>(0, 0);
    c1->force = vec2<float32_t>(0, 0);
    
    env.add_object(c1);    

    renderer<float32_t, vec2<float32_t>> r(env);

    r.init_frame();
    r.render();


    r.update_frame();
    r.render();

    std::cout<<"ok"<<std::endl;

    return 0;
}
