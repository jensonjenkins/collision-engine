#pragma once

#include "object.hpp"
#include <vector>

/**
 * @tparam T    numeric primitive type (e.g. float32_t, float64_t, etc.)
 * @tparam TV   vector wrapper defined in object.hpp (e.g. vec2, vec3)
 */
template <typename T, typename VT>
class environment {
private:
    static_assert(std::is_same_v<T, typename vec_traits<VT>::element_type>, 
            "both numeric primitive types passed must match.");
public:
    environment() {}
    
    void add_object(object<T, VT> *obj) { _objects.push_back(obj); }
    void remove_object(object<T, VT>* object) {}
    
    const std::vector<object<T, VT>*>& objects() const noexcept { return _objects; }

    void step(T dt) {
        for (object<T, VT>* obj : _objects) {
            obj->force += _gravity * obj->mass;
            obj->velocity += obj->force / obj->mass * dt;
            obj->position += obj->velocity * dt;
            obj->force.set_all(0);
        }
    }

private:
    const VT                    _gravity{0, -9.81f};
    std::vector<object<T, VT>*> _objects;
};
