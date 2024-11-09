#pragma once

#include "arm_neon.h"
#include <cassert>
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

    std::vector<uint32_t>& particles() noexcept { return _particles; }
    void add(uint32_t idx) { _particles.push_back(idx); }
    
private:    
    std::vector<uint32_t> _particles;
};

/**
 * @tparam PT (Particle Type) type of object the cell contains
 */
template <typename PT, typename W>
class grid {
public:
    /**
     * @param n_rows number of rows the grid contains
     * @param n_cols number of cols the grid contains
     */
    grid(W _world_size, uint32_t n_rows, uint32_t n_cols) noexcept 
        : _n_rows(n_rows), _n_cols(n_cols), _cell_width(_world_size.i() / _n_cols), _cell_height(_world_size.j()/ _n_rows) { 
        _cells.assign(_n_rows * _n_cols, cell<PT*>());
    }

    /**
     * Populates _grid from a 1-dimensional std::vector<T*> of particles
     */
    void populate(const std::vector<PT*>& particles) {
        _cells.assign(_n_rows * _n_cols, cell<PT*>());
        for (uint32_t idx = 0; idx < particles.size(); idx++) {
            uint32_t cell_id = get_cell_id(particles[idx]->position.i(), particles[idx]->position.j());
            _cells[cell_id].add(idx);
        }
    }

    uint32_t get_cell_id(float32_t i, float32_t j) {
        uint32_t i_cell = static_cast<uint32_t>(i) / _cell_height;
        uint32_t j_cell = static_cast<uint32_t>(j) / _cell_width;
        assert(i_cell >= 0 && i_cell < _n_rows && j_cell >= 0 && j_cell < _n_cols);
        return i_cell * _n_cols + j_cell;
    }

    inline uint32_t n_rows() const noexcept { return _n_rows; }
    inline uint32_t n_cols() const noexcept { return _n_cols; }
    inline std::vector<cell<PT*>>& cells() noexcept { return _cells; }

    
private: 
    const uint32_t          _n_rows;        // no. of rows on the grid
    const uint32_t          _n_cols;        // no. of cols on the grid
    const uint32_t          _cell_width;    // pixel width of each cell
    const uint32_t          _cell_height;   // pixel height of each cell
    std::vector<cell<PT*>>  _cells;
};

} // namespace collision_engine
