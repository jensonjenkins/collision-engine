#include "../src/physics/object.hpp"
#include "../src/physics/solver.hpp"
#include <arm_neon.h>
#include <iostream>


void step_function_test() {
    physics_solver<float32_t, vec2<float32_t>> solver;
    circle2d<float32_t> *c1 = new circle2d<float32_t>();
    c1->mass = 2;
    c1->position = vec2<float32_t>(0, 100);
    c1->velocity = vec2<float32_t>(0, 0);
    c1->force = vec2<float32_t>(0, 0);
    solver.add_object(c1);
    
    std::cout<<"mass: "<<c1->mass;
    std::cout<<"    x_i: "<<c1->position.i()<<"    x_j: "<<c1->position.j();
    std::cout<<"    v_i: "<<c1->velocity.i()<<"    v_j: "<<c1->velocity.j();
    std::cout<<"    f_i: "<<c1->force.j()<<"    f_j: "<<c1->force.j()<<std::endl;

    for (int i = 0; i < 100; i++) {
        solver.step(0.1);
    }
}

int main() { 
    std::cout<<"Running solver_test.cpp..."<<std::endl;

    step_function_test();

    return 0;
}
