#include "../src/physics/object.hpp"
#include "../src/physics/solver.hpp"
#include "../src/common/allocator.hpp"
#include <arm_neon.h>
#include <iostream>

namespace collision_engine {

void resolve_grid_collision_test() {
    using W = vec2<uint32_t>;
    using VT = vec2<float32_t>;
    using PT = particle<VT>;

    const uint32_t n_particles  = 110;

    linear_allocator lin_alloc(n_particles * sizeof(PT));

    W world_size(100, 100);
    environment<VT, W> env(world_size);

    for (uint32_t i = 0; i < 10; ++i) { // place 1 particle on each of the 100 cells
        for (uint32_t j = 0; j < 10; ++j) {
            void* allocated_memory = lin_alloc.allocate(sizeof(PT), alignof(PT));
            auto *p = new (allocated_memory) PT();
            p->radius = 1.5;
            p->position = VT(i * 10, j * 10);  
            env.add_particle(p);
        }
    }

    env.resolve_collisions();

    std::cout<<"\n1 - ok: particle v grid collision"<<std::endl; 
}

} // namespace collision_engine

int main() { 
    std::cout<<"==================================================================="<<std::endl;
    std::cout<<"Running solver_test.cpp..."<<std::endl;
    std::cout<<"==================================================================="<<std::endl;

    collision_engine::resolve_grid_collision_test();

    std::cout<<"==================================================================="<<std::endl;
    std::cout<<"solver_test - ok."<<std::endl;

    return 0;
}
