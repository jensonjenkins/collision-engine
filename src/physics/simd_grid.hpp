#pragma once

#include "simd_collection.hpp"
#include "arm_neon.h"
#include <cstdint>
#include <vector>

namespace collision_engine::simd {

template <typename T>
struct cell {
    cell() = default;
   
    size_t size;
    std::vector<uint32_t> ids;
    std::vector<T> xs, ys, rs; // temporary expendable storage
    
    void load_to_collection(particle_collection<T>& pc) noexcept {
        for(uint32_t i = 0; i < size; i++) {
            uint32_t id = ids[i];
            pc.xs[id] = xs[i];
            pc.ys[id] = ys[i];
        }
    }

    /**
     * @param id 
     * @param x 
     * @param y
     * @param r 
     */
    void add(uint32_t id, T x, T y, T r) {
        ids.push_back(id);
        xs.push_back(x);
        ys.push_back(y);
        rs.push_back(r);
        ++size;
    }
};

template <typename T>
struct grid {
    const uint32_t n_rows; // no. of rows on the grid
    const uint32_t n_cols; // no. of cols on the grid
                                  
    /**
     * @param world_width
     * @param world_height
     * @param n_rows
     * @param n_cols
     */
    grid(uint32_t world_height, uint32_t world_width, uint32_t n_rows, uint32_t n_cols) noexcept 
        : n_rows(n_rows), n_cols(n_cols), _cell_height(world_height / n_rows), _cell_width(world_width / n_cols) {
            _cells.assign(n_rows * n_cols, cell<T>());
        }
 
    void populate(particle_collection<T>& pc) {
        _cells.clear();
        _cells.assign(n_rows * n_cols, cell<T>());
        for (uint32_t idx = 0; idx < pc.xs.size(); idx++) {
            uint32_t cell_id = get_cell_id(pc.xs[idx], pc.ys[idx]);
            _cells[cell_id].add(idx, pc.xs[idx], pc.ys[idx], pc.rs[idx]);
        }
    }

    uint32_t get_cell_id(float32_t i, float32_t j) const noexcept {
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

    cell<T>& get_cell(uint32_t id) noexcept { return _cells[id]; }

private:
    const uint32_t          _cell_height;   // pixel height of each cell
    const uint32_t          _cell_width;    // pixel width of each cell
    std::vector<cell<T>>    _cells;
};

} // namespace collision engine

