#include "../src/physics/object.hpp"
#include "../src/physics/solver.hpp"
#include <arm_neon.h>
#include <iostream>


void step_function_test() {
    environment<float32_t, vec2<float32_t>> env;
    auto *c1 = new particle<float32_t, vec2<float32_t>>();

    c1->mass = 2;
    c1->radius = 2;
    c1->position = vec2<float32_t>(0, 100);
    c1->velocity = vec2<float32_t>(0, 0);
    c1->force = vec2<float32_t>(0, 0);
    
    std::cout<<"size of particle2d<float32_t>: "<<sizeof(particle<float32_t, vec2<float32_t>>)<<std::endl;
    std::cout<<"size of vec2<float32_t>: "<<sizeof(vec2<float32_t>)<<std::endl;

    env.add_particle(c1);    

    for (int i = 0; i < 100; i++) {
        env.step(0.1);
        std::cout<<"v_i: "<<c1->velocity.i()<<"    v_j: "<<c1->velocity.j();
        std::cout<<"    x_i: "<<c1->position.i()<<"    x_j: "<<c1->position.j()<<std::endl;
    }
}

int main() { 
    std::cout<<"Running solver_test.cpp..."<<std::endl;

    step_function_test();

    std::cout<<"solver_test - ok."<<std::endl;

    return 0;
}
