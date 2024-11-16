
#pragma once

#include "simd_collection.hpp"
#include "simd_grid.hpp"
#include <arm_neon.h>

namespace collision_engine {

template <typename T, typename W>
class simd_solver {    
private: 
    const uint32_t  _wx;
    const uint32_t  _wy;
    const uint32_t  _dt;
    float32_t       _sub_dt;
    simd::particle_collection<T>  _pc __attribute__((aligned(64))); 

    static constexpr T          _eps            = 0.001f;
    static constexpr T          _margin         = 4.f; 
    static constexpr T          _response_coef  = 4.f;
    static constexpr uint32_t   _sub_steps      = 2;

public:
    simd_solver(uint32_t wx, uint32_t wy, T dt) noexcept 
        : _wx(wx), _wy(wy), _dt(dt), _sub_dt(_dt / static_cast<float32_t>(_sub_steps)), _pc(1000, _sub_dt) {};

    void add_particle(const simd::particle<T>& p) noexcept { _pc.add(p); }
    void remove_particle(const simd::particle<T>& p) noexcept {};

    void resolve_particle_collision_simd(uint32_t p_idx, simd::cell<T> cell) const noexcept {
        float32x4_t p_x_reg = vdupq_n_f32(_pc.x[p_idx]);
        float32x4_t p_y_reg = vdupq_n_f32(_pc.y[p_idx]);
        float32x4_t p_r_reg = vdupq_n_f32(_pc.r[p_idx]);

        for (size_t offset = 0; offset < cell.size; offset += 4) {
        
        }

    }

    void step() {
        for(uint32_t i{_sub_steps}; i--;) {
            _pc.step();
        }
    }
}; 
    
} // namespace collision engine
