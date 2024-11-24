#include "../src/physics/simd_solver.hpp"
#include <arm_neon.h>
#include <iostream>

namespace collision_engine::simd {

void resolve_particle_collision_test() {
    constexpr float32_t dt  = 0.1f;

    f32_solver solver(dt); 
    for (int i = 0; i < 12; i++) {
        particle<float32_t> p(0.5 * i + 30, 30, 0.5 * i + 30, 30, 2);
        solver.add_particle(p);
    }
    // for(float32_t x : solver._pc.xs) { std::cout<<x<<" "; } std::cout<<std::endl;
    // for(float32_t y : solver._pc.ys) { std::cout<<y<<" "; } std::cout<<std::endl;
    
    solver.grid().populate(solver.pc());
    solver.resolve_particle_collision_simd(1, 0);

    // for(float32_t x : solver._pc.xs) { std::cout<<x<<" "; } std::cout<<std::endl;
    // for(float32_t y : solver._pc.ys) { std::cout<<y<<" "; } std::cout<<std::endl;

    std::cout<<"\n1 - ok: particle v cell collision"<<std::endl; 
}

void resolve_collision_test() {
    constexpr float32_t dt  = 0.01f;
    
    f32_solver solver(dt); 
    for (int i = 0; i < 120; i++) {
        particle<float32_t> p(0.5 * i + 30, 30, 0.5 * i + 30, 30, 2);
        solver.add_particle(p);
    }
    // for(float32_t x : solver._pc.xs) { std::cout<<x<<" "; } std::cout<<std::endl;
    // for(float32_t y : solver._pc.ys) { std::cout<<y<<" "; } std::cout<<std::endl;

    solver.grid().populate(solver.pc());
    solver.resolve_collision();

    // for(float32_t x : solver._pc.xs) { std::cout<<x<<" "; } std::cout<<std::endl;
    // for(float32_t y : solver._pc.ys) { std::cout<<y<<" "; } std::cout<<std::endl;

    std::cout<<"\n2 - ok: resolve collision test"<<std::endl; 
}

void step_test() {
    constexpr float32_t dt  = 0.01f;
    
    f32_solver solver(dt); 
    for (int i = 0; i < 120; i++) {
        particle<float32_t> p(0.5 * i + 30, 30, 0.5 * i + 30, 30, 2);
        solver.add_particle(p);
    }

    solver.step();

    std::cout<<"\n3 - ok: solver step test"<<std::endl; 
}

} // namespace collision_engine

int main() { 
    std::cout<<"==================================================================="<<std::endl;
    std::cout<<"Running simd_solver_test.cpp..."<<std::endl;
    std::cout<<"==================================================================="<<std::endl;

    collision_engine::simd::resolve_particle_collision_test();
    collision_engine::simd::resolve_collision_test();
    collision_engine::simd::step_test();

    std::cout<<"==================================================================="<<std::endl;
    std::cout<<"simd_solver_test - ok."<<std::endl;

    return 0;
}
