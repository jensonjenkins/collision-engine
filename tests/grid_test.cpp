#include "../src/physics/grid.hpp"
#include "../src/common/allocator.hpp"
#include "../src/physics/object.hpp"
#include <cassert>
#include <cstdint>
#include <iostream>

namespace collision_engine {

void populate_grid() { 
    using W = vec2<uint32_t>;
    using VT = vec2<float32_t>;
    using PT = particle<VT>;

    const uint32_t n_particles  = 110;

    std::vector<PT*> particles;
    linear_allocator lin_alloc(n_particles * sizeof(PT));

    W world_size(100, 100);
    grid<PT, W> g(world_size, 10, 10);

    for (uint32_t i = 0; i < 10; ++i) { // place 1 particle on each of the 100 cells
        for (uint32_t j = 0; j < 10; ++j) {
            void* allocated_memory = lin_alloc.allocate(sizeof(PT), alignof(PT));
            auto *p = new (allocated_memory) PT();
            p->position = VT(i * 10, j * 10);  
            particles.push_back(p);
        }
    }

    for (uint32_t i = 0; i < 10; ++i) { // arbitrarily place 10 more particles on pixel (64, 12)
        void* allocated_memory = lin_alloc.allocate(sizeof(PT), alignof(PT));
        auto *p = new (allocated_memory) PT();
        p->position = VT(64, 12);
        particles.push_back(p);
    }

    g.populate(particles);

    assert(g.n_cols() == 10);
    assert(g.n_rows() == 10);
    assert(g.cells().size() == 100 && "number of grid cells not 100.");

    for (uint32_t i = 0; i < g.cells().size(); ++i) {
        if(i == 61) { // the get_cell_id of pixel (64, 12) is 61, 
                      // so check of cell 61 has 10 more particles than the rest
            assert(g.cells()[i].particle_ids().size() == 11);
        } else {
            assert(g.cells()[i].particle_ids().size() == 1);
        }
    }
    
    std::cout<<"\n1 - ok: populate grid"<<std::endl;
} 
    
} //namespace collision engine

int main() {
    std::cout<<"==================================================================="<<std::endl;
    std::cout<<"Running grid_test.cpp..."<<std::endl;
    std::cout<<"==================================================================="<<std::endl;

    collision_engine::populate_grid();    

    std::cout<<"==================================================================="<<std::endl;
    std::cout<<"grid_test - ok."<<std::endl;
    
    return 0;
}
