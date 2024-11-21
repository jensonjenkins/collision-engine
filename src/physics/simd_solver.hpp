#pragma once

#include "simd_grid.hpp"
#include <iostream>
#include <arm_neon.h>

namespace collision_engine::simd {

template <typename T>
struct simd_solver {
    void step() {
        static_cast<T*>(this)->step_impl();
    }
}; 

class f32_solver : public simd_solver<f32_solver> {    
private: 
    using T = float32_t;

    const T         _sub_dt;
    const T         _dt;
    const uint32_t  _wx;
    const uint32_t  _wy;

    static constexpr T          _eps            = 0.001f;
    static constexpr T          _margin         = 4.f; 
    static constexpr T          _response_coef  = 4.f;
    static constexpr uint32_t   _sub_steps      = 2;

public:
    grid<T>                 _grid;
    particle_collection<T>  _pc __attribute__((aligned(64))); 

    f32_solver(uint32_t wx, uint32_t wy, uint32_t gx, uint32_t gy, T dt) noexcept 
        : _wx(wx), _wy(wy), _dt(dt), _sub_dt(dt / static_cast<T>(_sub_steps)), _pc(_sub_dt)
        , _grid(wx, wy, gx, gy) {};

    void add_particle(const particle<T>& p) noexcept { _pc.add(p); }
    void remove_particle(const particle<T>& p) noexcept {};
   
    void resolve_particle_collision_simd(uint32_t p_idx, cell<T> cell) noexcept {
        float32_t p_dx = 0, p_dy = 0;
        float32x4_t p_x_reg = vdupq_n_f32(_pc.xs[p_idx]);
        float32x4_t p_y_reg = vdupq_n_f32(_pc.ys[p_idx]);
        float32x4_t p_r_reg = vdupq_n_f32(_pc.rs[p_idx]);
            
        // TODO: exclude p_idx to be calculated against itself 
        for (size_t offset = 0; offset < cell.size; offset += 4) {
            std::cout<<offset<<std::endl;
            float32x4_t xs = vld1q_f32(cell.xs.data() + offset);
            float32x4_t ys = vld1q_f32(cell.ys.data() + offset);
            float32x4_t dxs = vsubq_f32(p_x_reg, xs);
            float32x4_t dys = vsubq_f32(p_y_reg, ys);

            float32x4_t dist = vsqrtq_f32(vaddq_f32(vmulq_f32(dxs, dxs), vmulq_f32(dys, dys)));

            float32x4_t rs = vld1q_f32(cell.rs.data() + offset);
            float32x4_t radius_sum = vaddq_f32(p_r_reg, rs);
            float32x4_t radius_ratio = vdivq_f32(rs, radius_sum);

            // vcltq_f32 sets ALL bits to 1 or 0 if p0 < p1, then resolves to all 1s;
            uint32x4_t mask = vcltq_f32(dist, radius_sum); 
            float32x4_t delta = vmulq_n_f32(vdivq_f32(vsubq_f32(radius_sum, dist), radius_sum), _response_coef);

            float32x4_t nx = vbslq_f32(mask, vmulq_f32(vdivq_f32(dxs, dist), delta), vdupq_n_f32(0));
            p_dx += vaddvq_f32(vmulq_f32(nx, radius_ratio));
            xs = vaddq_f32(xs, vmulq_f32(nx, radius_ratio));
            vst1q_f32(cell.xs.data() + offset, xs);

            float32x4_t ny = vbslq_f32(mask, vmulq_f32(vdivq_f32(dys, dist), delta), vdupq_n_f32(0)); 
            p_dy += vaddvq_f32(vmulq_f32(ny, radius_ratio));
            ys = vaddq_f32(ys, vmulq_f32(ny, radius_ratio));
            vst1q_f32(cell.ys.data() + offset, ys);
        } 
        // update collection data
        cell.load_to_collection(_pc);
        _pc.xs[p_idx] += p_dx;
        _pc.ys[p_idx] += p_dy;
    }

    void step_impl() {
        for(uint32_t i{_sub_steps}; i--;) {
            _grid.populate(_pc);
            _pc.step();
        }
    }
}; 
    
} // namespace collision engine
