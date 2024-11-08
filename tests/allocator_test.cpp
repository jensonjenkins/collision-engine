#include "../src/common/allocator.hpp"
#include "../src/physics/object.hpp"
#include <iostream>

namespace collision_engine {

void linear_allocator_single_allocation() {
    linear_allocator alloc(32);
    void* allocated_memory = alloc.allocate(sizeof(particle<vec2<float32_t>>), alignof(particle<vec2<float32_t>>));
    auto *p = new (allocated_memory) particle<vec2<float32_t>>();
    std::cout<<"\n1 - ok: linear_allocator: allocated single particle<vec2<float32_t>>\n"<<std::endl;
}

void linear_allocator_multple_allocation() {
    linear_allocator alloc(38400);
    for (uint32_t i = 0; i < 1200; ++i) {
        void* allocated_memory = alloc.allocate(sizeof(particle<vec2<float32_t>>), alignof(particle<vec2<float32_t>>));
        auto *p = new (allocated_memory) particle<vec2<float32_t>>();
        std::cout<<std::endl;
    }
    std::cout<<"2 - ok: linear_allocator: allocated 1200 particle<vec2<float32_t>>"<<std::endl;
}


}// namespace collision engine

int main() {
    std::cout<<"==================================================================="<<std::endl;
    std::cout<<"Running allocator_test.cpp..."<<std::endl;
    std::cout<<"==================================================================="<<std::endl;

    collision_engine::linear_allocator_single_allocation();
    collision_engine::linear_allocator_multple_allocation();

    std::cout<<"==================================================================="<<std::endl;
    std::cout<<"allocator_test - ok"<<std::endl;
    return 0;
}
