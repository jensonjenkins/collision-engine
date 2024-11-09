#include "../src/physics/object.hpp"
#include "../src/physics/solver.hpp"
#include <arm_neon.h>
#include <iostream>

namespace collision_engine {

void resolve_grid_collision_test() {
    
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
