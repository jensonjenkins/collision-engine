#include "../src/physics/grid.hpp"
#include "../src/physics/object.hpp"
#include <iostream>

namespace collision_engine{
    using PT = particle<vec2<float32_t>>;
    grid<PT> g(10, 10);
    
} //namespace collision engine

int main() {
    std::cout<<"==================================================================="<<std::endl;
    std::cout<<"Running grid_test.cpp..."<<std::endl;
    std::cout<<"==================================================================="<<std::endl;


    std::cout<<"==================================================================="<<std::endl;
    std::cout<<"grid_test - ok."<<std::endl;
    
    return 0;
}
