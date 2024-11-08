#pragma once

#include "arm_neon.h"

namespace collision_engine {

/**
 * @tparam T    numeric primitive type (e.g. float32_t, float64_t, etc.)
 * @tparam TV   vector wrapper defined in particle.hpp (e.g. vec2, vec3)
 */
template <typename T>
struct vec2 {};

template <>
struct vec2<float32_t> {
    float32x2_t data;

    float32_t i() const noexcept { return vget_lane_f32(data, 0); }
    float32_t j() const noexcept { return vget_lane_f32(data, 1); }
    void set_i(float32_t i) noexcept { data = vset_lane_f32(i, data, 0); }
    void set_j(float32_t j) noexcept { data = vset_lane_f32(j, data, 1); }
   
    vec2() = default;
    explicit vec2(float32x2_t data) : data(data) {} 
    explicit vec2(float32_t num) { data = vdup_n_f32(num); }
    explicit vec2(float32_t i, float32_t j) {
        data = vset_lane_f32(i, data, 0);
        data = vset_lane_f32(j, data, 1);
    }

    void set_all(float32_t num) { data = vdup_n_f32(num); }

    vec2& operator+=(const vec2& other) { data = vadd_f32(data, other.data); return *this; }
    vec2& operator-=(const vec2& other) { data = vsub_f32(data, other.data); return *this; }
    vec2& operator/=(const vec2& other) { data = vdiv_f32(data, other.data); return *this; }
    vec2& operator*=(const vec2& other) { data = vmul_f32(data, other.data); return *this; }

    friend vec2 operator+(vec2 a, const vec2& b) { a += b; return a; }
    friend vec2 operator-(vec2 a, const vec2& b) { a -= b; return a; }
    friend vec2 operator/(vec2 a, const vec2& b) { a /= b; return a; }
    friend vec2 operator*(vec2 a, const vec2& b) { a *= b; return a; }

    vec2& operator+=(float32_t num) { float32x2_t dup = vdup_n_f32(num); data = vadd_f32(data, dup); return *this; }
    vec2& operator-=(float32_t num) { float32x2_t dup = vdup_n_f32(num); data = vsub_f32(data, dup); return *this; }
    vec2& operator/=(float32_t num) { float32x2_t dup = vdup_n_f32(num); data = vmul_f32(data, dup); return *this; }
    vec2& operator*=(float32_t num) { float32x2_t dup = vdup_n_f32(num); data = vdiv_f32(data, dup); return *this; }

    vec2 operator+(float32_t num) const { float32x2_t dup = vdup_n_f32(num); return vec2(vadd_f32(data, dup)); }
    vec2 operator-(float32_t num) const { float32x2_t dup = vdup_n_f32(num); return vec2(vsub_f32(data, dup)); }
    vec2 operator/(float32_t num) const { float32x2_t dup = vdup_n_f32(num); return vec2(vdiv_f32(data, dup)); }
    vec2 operator*(float32_t num) const { float32x2_t dup = vdup_n_f32(num); return vec2(vmul_f32(data, dup)); }

    friend bool operator==(const vec2& a, const vec2& b) { return a.i() == b.i() && a.j() == b.j(); }
    friend bool operator!=(const vec2& a, const vec2& b) { return !(a == b); }

};

} // namespace collision engine
