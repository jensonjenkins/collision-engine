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
    
    /**
     * Update the particle collection with current cell data
     *  
     * @param pc collection of particle 
     */
    void load_to_collection(particle_collection<T>& pc) noexcept {
        for(uint32_t i = 0; i < size; i++) {
            uint32_t id = ids[i];
            pc.xs[id] = xs[i];
            pc.ys[id] = ys[i];
        }
    }

    /**
     * Add (the state of) a particle to the cell
     *
     * @param id    index of the particle in the main particle collection
     * @param x     x-coordinate of the particle
     * @param y     y-coordinate of the particle
     * @param r     radius of particle
     */
    void add(uint32_t id, T x, T y, T r) {
        ids.push_back(id);
        xs.push_back(x);
        ys.push_back(y);
        rs.push_back(r);
        ++size;
    }
};

/**
 * @tparam T    primitive data type of the grid
 * @tparam WH   world height in pixels (must be power of 2)
 * @tparam WW   world width in pixels (must be power of 2)
 * @tparam R    number of rows 
 * @tparam C    number of cols
 */
template <typename T, uint32_t WH, uint32_t WW, uint32_t R, uint32_t C>
struct grid {
private:
    constexpr bool is_power_of_2(uint32_t n) { return (n > 0) && ((n & (n - 1)) == 0); }
    constexpr uint32_t log2_constexpr(uint32_t n) { return (n > 1) ? 1 + log2_constexpr(n >> 1) : 0; }
    uint32_t  _cell_height_log2;
    uint32_t  _cell_width_log2;

#define STATIC_ASSERT_POWER_OF_2(n) static_assert(is_power_of_2(n), "Number is not a power of 2");

public:             
    constexpr grid() noexcept {
        // STATIC_ASSERT_POWER_OF_2(WH); 
        // STATIC_ASSERT_POWER_OF_2(WW);
        // STATIC_ASSERT_POWER_OF_2(R); 
        // STATIC_ASSERT_POWER_OF_2(C);
        
        _cell_height_log2 = log2_constexpr(WH / R);   // log_2(pixel height) of each cell
        _cell_width_log2 = log2_constexpr(WW / C);    // log_2(pixel width) of each cell

        _cells.assign(R * C, cell<T>());
    }
    
    /**
     * Populate the grid with particles
     *
     * @param pc collection of particle to populate the grid
     */
    constexpr void populate(particle_collection<T>& pc) {
        _cells.clear();
        _cells.assign(R * C, cell<T>());
        for (uint32_t idx = 0; idx < pc.xs.size(); idx++) {
            uint32_t cell_id = get_cell_id(pc.xs[idx], pc.ys[idx]);
            _cells[cell_id].add(idx, pc.xs[idx], pc.ys[idx], pc.rs[idx]);
        }
    }

    constexpr uint32_t get_cell_id(float32_t i, float32_t j) const noexcept {
        uint32_t i_cell = static_cast<uint32_t>(i) >> _cell_height_log2;
        uint32_t j_cell = static_cast<uint32_t>(j) >> _cell_width_log2;
        if (i_cell == R) { i_cell--; }
        if (j_cell == C) { j_cell--; }
        return i_cell * C + j_cell;
    }

    bool is_valid_cell(uint32_t cell_id) const noexcept { return (cell_id >= 0 && cell_id < R * C); }

    cell<T>& get_cell(uint32_t cell_id) noexcept { return _cells[cell_id]; }

private:
    std::vector<cell<T>> _cells;
};

} // namespace collision engine

