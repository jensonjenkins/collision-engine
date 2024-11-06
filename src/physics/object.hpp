#pragma once

#include "vec.hpp"
#include "arm_neon.h"

/**
 * @tparam T    floating point type (e.g. float32_t, float64_t, etc.) 
 * @tparam TV   vector wrapper defined in particle.hpp (e.g. vec2, vec3)
 */
template <typename T, typename VT>
struct particle {
    VT  position;
    VT  velocity;
    VT  force;
    T   mass;
    T   radius;

    particle() = default;
} __attribute__((__packed__, aligned(32))); 


static_assert(sizeof(particle<float32_t, vec2<float32_t>>) == 32 /* bytes */); 

template <typename T>
struct vec_traits;

/**
 * Template specialization of vec2 type traits
 */
template <>
struct vec_traits<vec2<float32_t>> {
    using element_type = float32_t;
    static constexpr uint8_t n_dims = 2;
};



