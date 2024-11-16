#include "arm_neon.h"
#include <cstdlib>

namespace collision_engine {

template <typename T>
struct particle {
    T x, y, px, py, r;
    particle(T x, T y, T px, T py, T r) : x(x), y(y), px(px), py(py), r(r) {};
} __attribute__((aligned(64)));

/**
 * @tparam T type of primitive (e.g. float32_t, uint32_t, etc.)
 */
template <typename T>
struct particle_collection {} __attribute__((aligned(64)));

template <>
struct particle_collection<float32_t> {
public:
    size_t      allocated_sz, used_sz{0}; 
    float32x4_t axs, ays;
    float32_t   *xs, *ys, *pxs, *pys, *rs, *x_buffer, *y_buffer, dt;

    const float32x4_t VELOCITY_DAMPING_REG = vdupq_n_f32(40.f);
    const float32x4_t DT_SQ_REG;

    particle_collection(size_t size, float32_t dt) : allocated_sz(size), dt(dt), DT_SQ_REG(vdupq_n_f32(dt * dt)) {
        pys         = static_cast<float32_t*>(std::malloc(allocated_sz * sizeof(float32_t)));
        pxs         = static_cast<float32_t*>(std::malloc(allocated_sz * sizeof(float32_t)));
        xs          = static_cast<float32_t*>(std::malloc(allocated_sz * sizeof(float32_t)));
        ys          = static_cast<float32_t*>(std::malloc(allocated_sz * sizeof(float32_t)));
        x_buffer    = static_cast<float32_t*>(std::malloc(allocated_sz * sizeof(float32_t)));
        y_buffer    = static_cast<float32_t*>(std::malloc(allocated_sz * sizeof(float32_t)));
        rs          = static_cast<float32_t*>(std::malloc(allocated_sz * sizeof(float32_t)));
    }

    ~particle_collection() {
        std::free(pys); std::free(pxs);
        std::free(xs); std::free(ys);
        std::free(x_buffer); std::free(y_buffer);
        std::free(rs);
    };
    
    void add(const particle<float32_t>& p) {
        if (used_sz < allocated_sz) {
            xs[used_sz] = p.x;
            ys[used_sz] = p.y;
            pxs[used_sz] = p.px;
            pys[used_sz] = p.py;
            rs[used_sz] = p.r;
            used_sz++;
        } 
    }

    /**
     * Increments new position of all particles
     */
    void step() {
        for (size_t offset = 0; offset < used_sz; offset += 4) {
            single_dim_verlet_update(pxs, xs, axs, offset, x_buffer);
            single_dim_verlet_update(pys, ys, ays, offset, y_buffer);
        }
        // update previous and current positions with buffer
        float32_t *temp_buffer_x = pxs, *temp_buffer_y = pys;
        pxs = xs; 
        pys = ys;
        xs = x_buffer; 
        ys = y_buffer;
        x_buffer = temp_buffer_x; 
        y_buffer = temp_buffer_y; 
    }

private:
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



