#pragma once

#include "arm_neon.h"
#include <vector>

namespace collision_engine::simd {

template <typename T>
struct cell {
    cell() = default;
   
    size_t size;
    std::vector<T> xs, ys, r; 
};

template <typename T>
class grid {};

} // namespace collision engine

