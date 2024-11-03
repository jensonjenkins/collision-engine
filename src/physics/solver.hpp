#pragma once

#include "object.hpp"
#include <iostream>
#include <vector>

/**
 * @tparam T    numeric primitive type (e.g. float32_t, float64_t, etc.)
 * @tparam TV   vector wrapper defined in object.hpp (e.g. vec2, vec3)
 */
template <typename T, typename VT>
class physics_solver {
private:
    static_assert(std::is_same_v<T, typename vec_traits<VT>::type>, 
            "both numeric primitive types passed must match.");
public:
    physics_solver() {}
    
    void add_object(object<T, VT>* obj) { 
        // _objects.push_back(obj); 
        std::cout<<"mass: "<<obj->mass;
        std::cout<<"    x_i: "<<obj->position.i()<<"    x_j: "<<obj->position.j();
        std::cout<<"    v_i: "<<obj->velocity.i()<<"    v_j: "<<obj->velocity.j();
        std::cout<<"    f_i: "<<obj->force.j()<<"    f_j: "<<obj->force.j()<<std::endl;
    }
    void remove_object(object<T, VT>* object) {}

    void step(T dt) {
        for (object<T, VT>* obj : _objects) {
            obj->force += _gravity * obj->mass;
            obj->velocity += obj->force / obj->mass * dt;
            obj->position += obj->velocity * dt;
            obj->force.set_all(0);

            // std::cout<<"mass: "<<obj->mass;
            // std::cout<<"    g_i: "<<_gravity.i()<<"    g_j: "<<_gravity.j();
            // std::cout<<"    x_i: "<<obj->position.i()<<"    x_j: "<<obj->position.j();
            // std::cout<<"    v_i: "<<obj->velocity.i()<<"    v_j: "<<obj->velocity.j();
            // std::cout<<"    f_i: "<<obj->force.j()<<"    f_j: "<<obj->force.j()<<std::endl;

        }
    }

private:
    const VT                    _gravity{0, -9.81f};
    std::vector<object<T, VT>*> _objects;
};
