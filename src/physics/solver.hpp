#pragma once

#include "object.hpp"
#include <arm_neon.h>
#include <cstdint>
#include <vector>

/**
 * @tparam VT vector wrapper defined in particle.hpp (e.g. vec2, vec3)
 */
template <typename VT>
class environment {
private: 
    using T = typename vec_traits<VT>::element_type;
public:
    environment() noexcept = default;
    
    void add_particle(particle<VT> *obj) { _particles.push_back(obj); }
    void remove_particle(particle<VT>* particle) {}
    
    const std::vector<particle<VT>*>& particles() const noexcept { return _particles; }
    
    void resolve_collision(uint32_t p1_idx, uint32_t p2_idx) {
        constexpr T response_coef   = 1.f;
        constexpr T eps             = 0.0001f;

        particle<VT>* p1 = _particles[p1_idx];
        particle<VT>* p2 = _particles[p2_idx];
        const T r1 = p1->radius;
        const T r2 = p2->radius;
        const T combined_radius = r1 + r2;

        const VT p2_p1 = p1->position - p2->position;
        const T dist_sq = (p2_p1.i() * p2_p1.i()) + (p2_p1.j() * p2_p1.j());

        if(dist_sq < combined_radius * combined_radius && dist_sq > eps) {
            const T dist = sqrt(dist_sq);
            const T delta = response_coef * (combined_radius - dist) / combined_radius;
            const VT col_vec = (p2_p1 / dist) * delta;

            p1->position += col_vec * (r2 / combined_radius);
            p2->position -= col_vec * (r1 / combined_radius);
        }
    }

    void step(T dt) {
        for (uint32_t i = 0; i < _particles.size(); i++) {
            for (uint32_t j = i + 1; j < _particles.size(); j++) {
                resolve_collision(i, j);
            }
        }
        for (auto *particle : _particles) {
            particle->acceleration += _gravity;
            particle->step(dt);
        }
        
    }

private:
    const VT                    _gravity{0, 40.f};
    std::vector<particle<VT>*>  _particles;
};
