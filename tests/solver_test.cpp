#include "../src/physics/object.hpp"
#include "../src/physics/solver.hpp"
#include <arm_neon.h>
#include <iomanip>
#include <iostream>

namespace collision_engine {

void step_function_test() {
    environment<vec2<float32_t>> env(vec2<float32_t>{300.f, 300.f});
    auto *c1 = new particle<vec2<float32_t>>();

    c1->mass = 2;
    c1->radius = 2;
    c1->position = vec2<float32_t>(100, 100);
    c1->prev_position= vec2<float32_t>(99.5, 100.05);
    
    std::cout<<"size of particle2d<float32_t>: "<<sizeof(particle<vec2<float32_t>>)<<std::endl;
    std::cout<<"size of vec2<float32_t>: "<<sizeof(vec2<float32_t>)<<std::endl;

    env.add_particle(c1);    

    for (int i = 0; i < 1000; i++) {
        env.step(0.005);
        std::cout<<std::setprecision(12);
        std::cout<<"    x_i: "<<c1->position.i()<<"    x_j: "<<c1->position.j();
        std::cout<<"    px_i: "<<c1->prev_position.i()<<"    px_j: "<<c1->prev_position.j()<<std::endl;
    }
}

} // namespace collision_engine

int main() { 
    std::cout<<"==================================================================="<<std::endl;
    std::cout<<"Running solver_test.cpp..."<<std::endl;
    std::cout<<"==================================================================="<<std::endl;

    collision_engine::step_function_test();

    std::cout<<"==================================================================="<<std::endl;
    std::cout<<"solver_test - ok."<<std::endl;

    return 0;
}
