
#pragma once

#include "simd_collection.hpp"
#include "simd_grid.hpp"
#include <arm_neon.h>

namespace collision_engine::simd {

template <typename T>
class simd_solver {}; 

template <>
class simd_solver<float32_t> {    
private: 
    using T = float32_t;

    T                       _sub_dt;
    const uint32_t          _wx;
    const uint32_t          _wy;
    const uint32_t          _dt;
    particle_collection<T>  _pc __attribute__((aligned(64))); 

    static constexpr T          _eps            = 0.001f;
    static constexpr T          _margin         = 4.f; 
    static constexpr T          _response_coef  = 4.f;
    static constexpr uint32_t   _sub_steps      = 2;

public:
    simd_solver(uint32_t wx, uint32_t wy, T dt) noexcept 
        : _wx(wx), _wy(wy), _dt(dt), _sub_dt(dt / static_cast<T>(_sub_steps)), _pc(_sub_dt) {};

    void add_particle(const particle<T>& p) noexcept { _pc.add(p); }
    void remove_particle(const particle<T>& p) noexcept {};
    
    void resolve_particle_collision_simd(uint32_t p_idx, simd::cell<T> cell) const noexcept {
        float32x4_t p_x_reg = vdupq_n_f32(_pc.xs[p_idx]);
        float32x4_t p_y_reg = vdupq_n_f32(_pc.ys[p_idx]);
        float32x4_t p_r_reg = vdupq_n_f32(_pc.rs[p_idx]);

        for (size_t offset = 0; offset < cell.size; offset += 4) {
            float32x4_t xs = vld1q_f32(cell.xs.data() + offset);
            float32x4_t ys = vld1q_f32(cell.ys.data() + offset);

            float32x4_t delta_xs  = vsubq_f32(p_x_reg, xs);
            float32x4_t delta_ys  = vsubq_f32(p_y_reg, ys);
            float32x4_t squared_delta_xs = vmulq_f32(delta_xs, delta_xs);
            float32x4_t squared_delta_ys = vmulq_f32(delta_ys, delta_ys);

            float32x4_t sum_deltas_squared = vaddq_f32(squared_delta_xs, squared_delta_ys);
            float32x4_t rs = vld1q_f32(cell.r.data() + offset);
            float32x4_t radius_sum = vaddq_f32(p_r_reg, rs);
            float32x4_t radius_sum_squared = vmulq_f32(radius_sum, radius_sum);

            uint32x4_t mask = vcltq_f32(sum_deltas_squared, radius_sum_squared); // vcltq_f32 sets ALL bits to 1 or 0
                                                                                // if p0 < p1, then resolves to all 1s;
 
        }
    }

    void step() {
        for(uint32_t i{_sub_steps}; i--;) {
            _pc.step();
        }
    }
}; 
    
} // namespace collision engine
