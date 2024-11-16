#pragma once

#include "object.hpp"
#include "grid.hpp"
#include <arm_neon.h>
#include <cstdint>
#include <vector>

namespace collision_engine {

/**
 * @tparam VT vector wrapper defined in particle.hpp (e.g. vec2, vec3)
 */
template <typename VT, typename W>
class environment {
private: 
    using T = typename vec_traits<VT>::element_type;
public:
    environment(W world_size) noexcept : _world_size(world_size), _grid{world_size, 128, 128} {};
    
    void add_particle(particle<VT> *p) noexcept { _particles.push_back(p); }
    void remove_particle(particle<VT>* p) {}
    
    const std::vector<particle<VT>*>& particles() const noexcept { return _particles; }
    
    void resolve_particle_collision(uint32_t p1_idx, uint32_t p2_idx) const noexcept {
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

    void resolve_cell_collision(uint32_t cell_id, uint32_t o_cell_id) {
        if (!_grid.is_valid_cell(o_cell_id)) {
            return;
        }
        std::vector<cell<particle<VT>*>>& cells = _grid.cells();
        std::vector<uint32_t>& cur_cell = cells[cell_id].particle_ids();
        std::vector<uint32_t>& o_cell = cells[o_cell_id].particle_ids();
        
        for (uint32_t c_particle_id : cur_cell) {
            for (uint32_t o_particle_id : o_cell) {
                if(c_particle_id == o_particle_id) continue;
                resolve_particle_collision(c_particle_id, o_particle_id); // resovlve 1v1 particle collision
            }
        }
    }
    
    void resolve_collisions() {
        for (uint32_t i = 0; i < _grid.n_rows; i++) {
            for (uint32_t j = 0; j < _grid.n_cols; j++) {
                uint32_t cell_id = i * _grid.n_cols + j;

                resolve_cell_collision(cell_id, cell_id);
                resolve_cell_collision(cell_id, cell_id - 1);
                resolve_cell_collision(cell_id, cell_id + 1);

                resolve_cell_collision(cell_id, cell_id + _grid.n_cols);
                resolve_cell_collision(cell_id, cell_id + _grid.n_cols - 1);
                resolve_cell_collision(cell_id, cell_id + _grid.n_cols + 1);

                resolve_cell_collision(cell_id, cell_id - _grid.n_cols);
                resolve_cell_collision(cell_id, cell_id - _grid.n_cols - 1);
                resolve_cell_collision(cell_id, cell_id - _grid.n_cols + 1);
            }
        }
    }

    void update_objects(T dt) {
        for (auto *particle : _particles) {
            particle->acceleration += _gravity;
            particle->step(dt); // update particle position and trajectory
            
            if (particle->position.i() > _world_size.i() - _margin) { // boundary checks
                particle->position.set_i(_world_size.i() - _margin);
            } else if (particle->position.i() < _margin) {
                particle->position.set_i(_margin);
            }

            if (particle->position.j() > _world_size.j() - _margin) { // boundary checks
                particle->position.set_j(_world_size.j() - _margin);
            } else if (particle->position.j() < _margin) {
                particle->position.set_j(_margin);
            }
        } 
    }
    
    void step(T dt) {
        const float32_t sub_dt = dt / static_cast<float32_t>(_sub_steps);
        for(uint32_t i{_sub_steps}; i--;) {
            _grid.populate(_particles);
            resolve_collisions();
            update_objects(sub_dt);
        }
    }

private:
    grid<particle<VT>, W>       _grid;
    std::vector<particle<VT>*>  _particles;

    // Constants
    W                           _world_size;
    const VT                    _gravity            {0, 98.1f};
    static constexpr T          _eps                = 0.001f;
    static constexpr T          _margin             = 4.f; 
    static constexpr T          _response_coef      = 4.f;
    static constexpr uint32_t   _sub_steps          = 2;
};

} // namespace collision engine
