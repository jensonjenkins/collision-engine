#pragma once

#include "object.hpp"
#include <arm_neon.h>
#include <cstdint>
#include <vector>

namespace collision_engine {
/**
 * @tparam VT vector wrapper defined in particle.hpp (e.g. vec2, vec3)
 */
template <typename VT>
class environment {
private: 
    using T = typename vec_traits<VT>::element_type;
public:
    environment(VT world_size) noexcept : _world_size(world_size) {};
    
    void add_particle(particle<VT> *p) noexcept { _particles.push_back(p); }
    void remove_particle(particle<VT>* p) {}
    
    const std::vector<particle<VT>*>& particles() const noexcept { return _particles; }
    
    void resolve_collision(uint32_t p1_idx, uint32_t p2_idx) const noexcept {
        particle<VT>* p1 = _particles[p1_idx];
        particle<VT>* p2 = _particles[p2_idx];
        const T r1 = p1->radius;
        const T r2 = p2->radius;
        const T combined_radius = r1 + r2;

        const VT p2_p1 = p1->position - p2->position;
        const T dist_sq = (p2_p1.i() * p2_p1.i()) + (p2_p1.j() * p2_p1.j());

        if(dist_sq < combined_radius * combined_radius && dist_sq > _eps) {
            const T dist = sqrt(dist_sq);
            const T delta = _response_coef * (combined_radius - dist) / combined_radius;
            const VT col_vec = (p2_p1 / dist) * delta;

            p1->position += col_vec * (r2 / combined_radius); // simulates momentum
            p2->position -= col_vec * (r1 / combined_radius); 
        }
    }

    void step(T dt) {
        for (uint32_t i = 0; i < _particles.size(); i++) {
            for (uint32_t j = i + 1; j < _particles.size(); j++) {
                resolve_collision(i, j); // update particle position if collision occurs
            }
        }
        for (auto *particle : _particles) {
            particle->acceleration += _gravity;
            particle->step(dt); // update particle position and trajectory
            
            if (particle->position.i() > _world_size.i() - _margin) {
                particle->position.set_i(_world_size.i() - _margin);
            } else if (particle->position.i() < _margin) {
                particle->position.set_i(_margin);
            }

            if (particle->position.j() > _world_size.j() - _margin) {
                particle->position.set_j(_world_size.j() - _margin);
            } else if (particle->position.j() < _margin) {
                particle->position.set_j(_margin);
            }
        } 
    }

private:
    std::vector<particle<VT>*>  _particles;

    // Constants
    static constexpr T  _eps             = 0.0001f;
    static constexpr T  _margin          = 2.f; 
    static constexpr T  _response_coef   = 4.f;
    const VT            _gravity{0, 98.1f};
    VT                  _world_size;
};

} // namespace collision engine
