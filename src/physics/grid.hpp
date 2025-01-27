#pragma once

#include "arm_neon.h"
#include <cstdint>
#include <vector>

namespace collision_engine {

/**
 * @tparam PT (Particle Type) type of object the cell contains
 */
template <typename PT>
class cell {
public:
    cell() = default;

    std::vector<uint32_t>& particle_ids() noexcept { return _particle_ids; }

    void add(uint32_t id) { _particle_ids.push_back(id); }
    
private:    
    std::vector<uint32_t> _particle_ids;
};

/**
 * @tparam PT (Particle Type) type of object the cell contains
 */
template <typename PT, typename W>
class grid {
private:
    const uint32_t          _cell_width;    // pixel width of each cell
    const uint32_t          _cell_height;   // pixel height of each cell
    std::vector<cell<PT*>>  _cells;

public:
    const uint32_t n_rows;  // no. of rows on the grid
    const uint32_t n_cols;  // no. of cols on the grid
    uint32_t cell_count;
                                            
    /**
     * @param world_size world size of vec type W
     * @param n_rows number of rows the grid contains
     * @param n_cols number of cols the grid contains
     */
    grid(W world_size, uint32_t n_rows, uint32_t n_cols) noexcept 
        : n_rows(n_rows), n_cols(n_cols), _cell_width(world_size.i() / n_cols), _cell_height(world_size.j()/ n_rows) { 
        _cells.assign(n_rows * n_cols, cell<PT*>());
        cell_count = n_cols * n_rows;
    }

    /**
     * Populates _grid from a 1-dimensional std::vector<T*> of particles
     */
    void populate(const std::vector<PT*>& particles) {
        _cells.clear();
        _cells.assign(n_rows * n_cols, cell<PT*>());
        for (uint32_t idx = 0; idx < particles.size(); idx++) {
            uint32_t cell_id = get_cell_id(particles[idx]->position.i(), particles[idx]->position.j());
            _cells[cell_id].add(idx);
        }
    }

    bool is_valid_cell(uint32_t cell_id) const noexcept {
        return (cell_id >= 0 && cell_id < n_rows * n_cols);
    }

    uint32_t get_cell_id(float32_t i, float32_t j) {
        uint32_t i_cell = static_cast<uint32_t>(i) / _cell_height;
        uint32_t j_cell = static_cast<uint32_t>(j) / _cell_width;
        if (i_cell == n_rows) {
            i_cell--;
        }
        if (j_cell == n_cols) {
            j_cell--;
        }
        return i_cell * n_cols + j_cell;
    }

    std::vector<cell<PT*>>& cells() noexcept { return _cells; }
};

} // namespace collision_engine
