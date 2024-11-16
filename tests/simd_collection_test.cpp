#include "../src/physics/simd_collection.hpp"
#include <arm_neon.h>
#include <cassert>
#include <iostream>

namespace collision_engine::simd {

void initialize_collection_test() {

    { // scope the collection to test destructor 
        particle_collection<float32_t> col(0.5);
    }

    std::cout<<"1 - ok: initialize collection"<<std::endl;
}

void add_particle_to_simd_collection_test() {

    { // scope the collection to test destructor
        particle_collection<float32_t> col(0.5);
        for (int i{10};i--;) {
            particle<float32_t> p(1, 2, 3, 4, 5);
            col.add(p);
        }
        for (int i=0;i<10;i++) {
            assert(col.xs[i] == 1);
            assert(col.ys[i] == 2);
            assert(col.pxs[i] == 3);
            assert(col.pys[i] == 4);
            assert(col.rs[i] == 5);
        }
        assert(col.xs.size() == 10);
    }

    std::cout<<"2 - ok: add particle to collection"<<std::endl; 
}

void single_dim_verlet_update_test() {
    { // scope the collection to test destructor
        particle_collection<float32_t> col(0.1);
        for (int i{8};i--;) {
            particle<float32_t> p(2, 2, 2, 2, 2);
            col.add(p);
        }
        col.single_dim_verlet_update(col.pxs.data(), col.xs.data(), col.ax_reg, 0, col.x_buffer.data());
        col.single_dim_verlet_update(col.pxs.data(), col.xs.data(), col.ax_reg, 4, col.x_buffer.data());
        col.single_dim_verlet_update(col.pys.data(), col.ys.data(), col.ay_reg, 0, col.y_buffer.data());
        col.single_dim_verlet_update(col.pys.data(), col.ys.data(), col.ay_reg, 4, col.y_buffer.data());
        
        for (int i=0;i<8;i++) { assert(col.x_buffer[i] == 2); }
        for (int i=0;i<8;i++) { assert(2.980 <= col.y_buffer[i] && col.y_buffer[i] <= 2.982); }
    }
    
    std::cout<<"3 - ok: single dim SIMD verlet update"<<std::endl; 
}

void step_function_test() {
    { // scope the collection to test destructor
        particle_collection<float32_t> col(0.1);
        for (int i{8};i--;) {
            particle<float32_t> p(2, 2, 0, 0, 1);
            col.add(p);
        }

        col.step();

        for (int i=0;i<8;i++) { assert(col.pxs[i] == 2); }
        for (int i=0;i<8;i++) { assert(col.pys[i] == 2); }
        for (int i=0;i<8;i++) { assert(3.199 <= col.xs[i] && col.xs[i] <= 3.201); }
        for (int i=0;i<8;i++) { assert(4.180 <= col.ys[i] && col.ys[i] <= 4.182); }
    }

    std::cout<<"4 - ok: step function"<<std::endl; 
}

} // namespace collision_engine

int main() { 
    std::cout<<"==================================================================="<<std::endl;
    std::cout<<"Running simd_collection_test.cpp..."<<std::endl;
    std::cout<<"==================================================================="<<std::endl;

    collision_engine::simd::initialize_collection_test();
    collision_engine::simd::add_particle_to_simd_collection_test();
    collision_engine::simd::single_dim_verlet_update_test();
    collision_engine::simd::step_function_test();

    std::cout<<"==================================================================="<<std::endl;
    std::cout<<"simd_collection_test - ok."<<std::endl;

    return 0;
}

