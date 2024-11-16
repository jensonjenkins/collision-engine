#include "arm_neon.h"
#include <vector>
#include <cstdlib>

namespace collision_engine::simd {

template <typename T>
struct particle {
    T x, y, px, py, r;
    particle(T x, T y, T px, T py, T r) : x(x), y(y), px(px), py(py), r(r) {};
    ~particle() {};
} __attribute__((aligned(64)));

/**
 * @tparam T type of primitive (e.g. float32_t, uint32_t, etc.)
 */
template <typename T>
struct particle_collection {} __attribute__((aligned(64)));

template <>
struct particle_collection<float32_t> {
private:
    const float32x4_t VELOCITY_DAMPING_REG  = vdupq_n_f32(40.f);
    const float32x4_t DT_SQ_REG;

public:
    float32_t dt;
    std::vector<float32_t> xs, ys, pxs, pys, rs, x_buffer, y_buffer;

    const float32x4_t ax_reg = vdupq_n_f32(0.f); 
    const float32x4_t ay_reg = vdupq_n_f32(98.1f); // gravity

    particle_collection(float32_t dt) : dt(dt), DT_SQ_REG(vdupq_n_f32(dt * dt)) {}

    ~particle_collection() {};
    
    void add(const particle<float32_t>& p) {
        xs.push_back(p.x);
        ys.push_back(p.y);
        pxs.push_back(p.px);
        pys.push_back(p.py);
        rs.push_back(p.r);
        x_buffer.push_back(0); // fill buffer with dummy data
        y_buffer.push_back(0);
    }

    /**
     * Increments new position of all particles
     */
    void step() {
        for (size_t offset = 0; offset < xs.size(); offset += 4) {
            single_dim_verlet_update(pxs.data(), xs.data(), ax_reg, offset, x_buffer.data());
        }
        for (size_t offset = 0; offset < xs.size(); offset += 4) {
            single_dim_verlet_update(pys.data(), ys.data(), ay_reg, offset, y_buffer.data()); 
        }
        // update previous and current positions with buffer
        // (cycles around 3 buffers)
        std::vector<float32_t> temp_buffer_x = std::move(pxs); 
        pxs = std::move(xs); 
        xs = std::move(x_buffer); 
        x_buffer = std::move(temp_buffer_x); 

        std::vector<float32_t> temp_buffer_y = std::move(pys);
        pys = std::move(ys);
        ys = std::move(y_buffer);
        y_buffer = std::move(temp_buffer_y); 
    }

    /**
     * Perform updates for a single set of float32x4_t register 
     * for a single dimension (i.e. either x or y if its 2D) to be called
     * repeatedly within step(float32_t dt). 
     *
     * The updates resemble the 
     * verlet integration formula: x(t + dt) = x(t) + v(t)dt + a(t) * dt^2;
     *
     * @param ps        previous position of particles
     * @param cs        current position of particles
     * @param offset    offset to location in memory
     */
    void single_dim_verlet_update(float32_t* ps, float32_t* cs, float32x4_t a_reg, size_t offset, float32_t* buffer) {
        float32x4_t p_reg = vld1q_f32(ps + offset);     // prev position register
        float32x4_t c_reg = vld1q_f32(cs + offset);     // current position register

        float32x4_t d_reg = vsubq_f32(c_reg, p_reg);    // position delta/difference register
        float32x4_t n_reg = vaddq_f32(c_reg, d_reg);    // new position register 

        // n_reg is currently x(t) + v(t)dt
        // the line below adds a(t) * dt^2 to n_reg
        n_reg = vaddq_f32(n_reg, vmulq_f32(vsubq_f32(a_reg, vmulq_f32(d_reg, VELOCITY_DAMPING_REG)), DT_SQ_REG));
        
        buffer[offset]      = vgetq_lane_f32(n_reg, 0);
        buffer[offset + 1]  = vgetq_lane_f32(n_reg, 1);
        buffer[offset + 2]  = vgetq_lane_f32(n_reg, 2);
        buffer[offset + 3]  = vgetq_lane_f32(n_reg, 3);
    }

} __attribute__((aligned(64)));


} // namespace collision_engine



