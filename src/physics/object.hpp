#pragma once

#include "arm_neon.h"

/**
 * Wrapper class for float32x2_t and float64x2_t represented as 2D vectors 
 */
template <typename T>
struct vec2 {};

template <>
struct vec2<float32_t> {
    float32x2_t data;

    float32_t i() const noexcept { return vget_lane_f32(data, 0); }
    float32_t j() const noexcept { return vget_lane_f32(data, 1); }
   
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

    vec2& operator+=(float32_t num) { float32x2_t dup = vdup_n_f32(num); vadd_f32(data, dup); return *this; }
    vec2& operator-=(float32_t num) { float32x2_t dup = vdup_n_f32(num); vsub_f32(data, dup); return *this; }
    vec2& operator/=(float32_t num) { float32x2_t dup = vdup_n_f32(num); vmul_f32(data, dup); return *this; }
    vec2& operator*=(float32_t num) { float32x2_t dup = vdup_n_f32(num); vdiv_f32(data, dup); return *this; }

    vec2 operator+(float32_t num) const { float32x2_t dup = vdup_n_f32(num); return vec2(vadd_f32(data, dup)); }
    vec2 operator-(float32_t num) const { float32x2_t dup = vdup_n_f32(num); return vec2(vsub_f32(data, dup)); }
    vec2 operator/(float32_t num) const { float32x2_t dup = vdup_n_f32(num); return vec2(vdiv_f32(data, dup)); }
    vec2 operator*(float32_t num) const { float32x2_t dup = vdup_n_f32(num); return vec2(vmul_f32(data, dup)); }

    friend bool operator==(const vec2& a, const vec2& b) { return a.i() == b.i() && a.j() == b.j(); }
    friend bool operator!=(const vec2& a, const vec2& b) { return !(a == b); }

} __attribute__((aligned(64)));

template <typename T>
struct vec_traits;

/**
 * Template specialization of vec2 type traits
 */
template <>
struct vec_traits<vec2<float32_t>> {
    using type = float32_t;
    static constexpr uint8_t dims = 2;
};

template <typename T, typename VT>
struct object {
    T   mass;
    VT  position;
    VT  velocity;
    VT  force;
};

/**
 * @tparam T floating point type (e.g. float32_t, float64_t, etc.) 
 */
template <typename T>
struct object2d : public object<T, vec2<T>> {
    T       mass;
    vec2<T> position;
    vec2<T> velocity;
    vec2<T> force;

    object2d() = default;
};

template <typename T>
struct circle2d : public object2d<T> {};


