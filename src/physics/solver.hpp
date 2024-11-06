#pragma once

#include "object.hpp"
#include <vector>

/**
 * @tparam T    numeric primitive type (e.g. float32_t, float64_t, etc.)
 * @tparam TV   vector wrapper defined in particle.hpp (e.g. vec2, vec3)
 */
template <typename T, typename VT>
class environment {
private:
    static_assert(std::is_same_v<T, typename vec_traits<VT>::element_type>, 
            "both numeric primitive types passed must match.");
public:
    environment() {}
    
    void add_particle(particle<T, VT> *obj) { _particles.push_back(obj); }
    void remove_particle(particle<T, VT>* particle) {}
    
    const std::vector<particle<T, VT>*>& particles() const noexcept { return _particles; }
    
    void resolve_collision(uint32_t obj_1_idx, uint32_t obj_2_idx) {
        
    }

    void step(T dt) {
        vec2<T> next_pos;
        for (particle<T, VT>* obj : _particles) {
            obj->force += _gravity * obj->mass;
            obj->velocity += obj->force / obj->mass * dt;
            obj->position += obj->velocity * dt;
            obj->force.set_all(0);
        }
    }


private:
    const VT                    _gravity{0, 9.81f};
    std::vector<particle<T, VT>*> _particles;
};
