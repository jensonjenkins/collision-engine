#include "../src/physics/simd_solver.hpp"
#include <arm_neon.h>
#include <iostream>

namespace collision_engine::simd {

void resolve_particle_collision_test() {
    f32_solver solver(512, 512, 1, 1, 0.1); 
    for (int i = 0; i < 4; i++) {
        particle<float32_t> p(0.5 * i + 30, 30, 0.5 * i + 30, 30, 2);
        solver.add_particle(p);
    }
    // for(float32_t x : solver._pc.xs) { std::cout<<x<<" "; } std::cout<<std::endl;
    // for(float32_t x : solver._pc.ys) { std::cout<<x<<" "; } std::cout<<std::endl;
    
    solver._grid.populate(solver._pc);
    for(uint32_t x : solver._grid.get_cell(0).ids) { std::cout<<x<<" "; } std::cout<<std::endl;
    for(float32_t x : solver._grid.get_cell(0).xs) { std::cout<<x<<" "; } std::cout<<std::endl;
    for(float32_t x : solver._grid.get_cell(0).ys) { std::cout<<x<<" "; } std::cout<<std::endl;
    for(float32_t x : solver._grid.get_cell(0).rs) { std::cout<<x<<" "; } std::cout<<std::endl;
    solver.resolve_particle_collision_simd(1, solver._grid.get_cell(0));

    for(float32_t x : solver._pc.xs) { std::cout<<x<<" "; } std::cout<<std::endl;
    for(float32_t x : solver._pc.ys) { std::cout<<x<<" "; } std::cout<<std::endl;

    std::cout<<"\n1 - ok: particle v cell collision"<<std::endl; 
}

} // namespace collision_engine

int main() { 
    std::cout<<"==================================================================="<<std::endl;
    std::cout<<"Running simd_solver_test.cpp..."<<std::endl;
    std::cout<<"==================================================================="<<std::endl;

    collision_engine::simd::resolve_particle_collision_test();

    std::cout<<"==================================================================="<<std::endl;
    std::cout<<"simd_solver_test - ok."<<std::endl;

    return 0;
}
