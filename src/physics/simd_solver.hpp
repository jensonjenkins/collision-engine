#pragma once

#include "simd_grid.hpp"
#include <arm_neon.h>

namespace collision_engine::simd {

template <typename T>
struct simd_solver {
    void step() { static_cast<T*>(this)->step_impl(); }
}; 

class f32_solver : public simd_solver<f32_solver> {    
private: 
    using T = float32_t;

    const T         _sub_dt;
    const T         _dt;

    static constexpr T          _eps            = 0.001f;
    static constexpr T          _response_coef  = 4.f;
    static constexpr uint32_t   _sub_steps      = 2;
    static constexpr uint32_t   _WW             = 128;
    static constexpr uint32_t   _WH             = 128;
    static constexpr uint32_t   _C              = 32;
    static constexpr uint32_t   _R              = 32; 

    grid<T, _WH, _WW, _R, _C>   _grid;
    particle_collection<T>      _pc __attribute__((aligned(64))); 

public:

    f32_solver(T dt) noexcept : _dt(dt), _sub_dt(dt / static_cast<T>(_sub_steps)), _pc(_WW, _WH, _sub_dt), _grid() {};

    void add_particle(const particle<T>& p) noexcept { _pc.add(p); }
    void remove_particle(const particle<T>& p) noexcept {};

    particle_collection<T>& pc() noexcept { return _pc; }
    grid<T, _WH, _WW, _R, _C>& grid() noexcept { return _grid; }
 
    /**
     * Resolves collision for 1 particle against all particles in a given cell (identified by cell_id)
     *
     * @param p_idx     index of the single particle
     * @param cell_id   index of cell to resolve collision with
     */
    void resolve_particle_collision_simd(uint32_t p_idx, uint32_t cell_id) noexcept {
        if (!_grid.is_valid_cell(cell_id)) {
            return;
        }
        cell<T>& cell = _grid.get_cell(cell_id);

        float32_t p_dx = 0, p_dy = 0;
        float32x4_t p_x_reg = vdupq_n_f32(_pc.xs[p_idx]);
        float32x4_t p_y_reg = vdupq_n_f32(_pc.ys[p_idx]);
        float32x4_t p_r_reg = vdupq_n_f32(_pc.rs[p_idx]);
        uint32x4_t p_id_reg = vdupq_n_u32(p_idx);
            
        for (size_t offset = 0; offset < cell.size; offset += 4) {
            float32x4_t xs = vld1q_f32(cell.xs.data() + offset);
            float32x4_t ys = vld1q_f32(cell.ys.data() + offset);

            float32x4_t dxs = vsubq_f32(p_x_reg, xs);
            float32x4_t dys = vsubq_f32(p_y_reg, ys);

            float32x4_t dist = vsqrtq_f32(vaddq_f32(vmulq_f32(dxs, dxs), vmulq_f32(dys, dys)));

            float32x4_t rs = vld1q_f32(cell.rs.data() + offset);
            float32x4_t radius_sum = vaddq_f32(p_r_reg, rs);
            float32x4_t radius_ratio = vdivq_f32(rs, radius_sum);

            float32x4_t delta = vmulq_n_f32(vdivq_f32(vsubq_f32(radius_sum, dist), radius_sum), _response_coef);

            // vcltq_f32 sets ALL bits to 1 or 0 if p0 < p1, then resolves to all 1s;
            uint32x4_t mask_lt = vcltq_f32(dist, radius_sum); 
            // vcgtq_f32 sets ALL bits to 1 or 0 if p0 > p1, then resolves to all 1s;
            uint32x4_t mask_gt = vcgtq_f32(dist, vdupq_n_f32(0.001f)); 
            uint32x4_t ids = vld1q_u32(cell.ids.data() + offset);
            uint32x4_t neq_id_mask = vmvnq_u32(vceqq_u32(p_id_reg, ids));
            uint32x4_t mask = vandq_u32(vandq_u32(mask_lt, mask_gt), neq_id_mask);

            float32x4_t nx = vbslq_f32(mask, vmulq_f32(vdivq_f32(dxs, dist), delta), vdupq_n_f32(0));
            p_dx += vaddvq_f32(vmulq_f32(nx, radius_ratio));
            xs = vsubq_f32(xs, vmulq_f32(nx, radius_ratio));
            vst1q_f32(cell.xs.data() + offset, xs);

            float32x4_t ny = vbslq_f32(mask, vmulq_f32(vdivq_f32(dys, dist), delta), vdupq_n_f32(0)); 
            p_dy += vaddvq_f32(vmulq_f32(ny, radius_ratio));
            ys = vsubq_f32(ys, vmulq_f32(ny, radius_ratio));
            vst1q_f32(cell.ys.data() + offset, ys);
        } 
        // update collection data
        cell.load_to_collection(_pc);
        _pc.xs[p_idx] += p_dx;
        _pc.ys[p_idx] += p_dy;
    }
    
    void resolve_collision() noexcept {
        for (uint32_t p_id = 0; p_id < _pc.xs.size(); p_id++) {
            uint32_t p_cell_id = _grid.get_cell_id(_pc.xs[p_id], _pc.ys[p_id]);
            resolve_particle_collision_simd(p_id, p_cell_id);
            resolve_particle_collision_simd(p_id, p_cell_id - 1);
            resolve_particle_collision_simd(p_id, p_cell_id + 1);

            resolve_particle_collision_simd(p_id, p_cell_id + _C);
            resolve_particle_collision_simd(p_id, p_cell_id + _C - 1);
            resolve_particle_collision_simd(p_id, p_cell_id + _C + 1);

            resolve_particle_collision_simd(p_id, p_cell_id - _C);
            resolve_particle_collision_simd(p_id, p_cell_id - _C - 1);
            resolve_particle_collision_simd(p_id, p_cell_id - _C + 1);
        }
    }

    void step_impl() {
        for(uint32_t i{_sub_steps}; i--;) {
            _grid.populate(_pc);
            resolve_collision();
            _pc.step();
        }
    }
}; 
    
} // namespace collision engine
