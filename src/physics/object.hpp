#pragma once

#include "vec.hpp"
#include "arm_neon.h"

namespace collision_engine {

/**
 * @tparam T floating point type (e.g. float32_t, float64_t, etc.) 
 */
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

/**
 * @tparam VT vector wrapper defined in particle.hpp (e.g. vec2, vec3)
 */
template <typename VT>
struct particle {
    using T = typename vec_traits<VT>::element_type;

    VT  position;
    VT  prev_position;
    VT  acceleration;
    T   mass;
    T   radius;

    particle() = default;

    void step(T dt) {
        constexpr T VELOCITY_DAMPING = 15.f; // approximates air resistance 
        const VT displacement = position - prev_position;

        // Verlet integration:
        // x(t + dt) = x(t) + v(t)dt + a(t) * t * t
        const VT new_position = position + displacement + (acceleration - displacement * VELOCITY_DAMPING) * (dt * dt);
        prev_position = position;
        position = new_position;
        acceleration = VT{0.f, 0.f};
    }        

} __attribute__((__packed__, aligned(32))); 

static_assert(sizeof(particle<vec2<float32_t>>) == 32 /* bytes */); 

} // namespace collision engine


