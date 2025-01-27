#pragma once

#include <memory>

namespace collision_engine {

template <typename AllocatorImpl>
class allocator {
public:
    allocator() = default;
    allocator(std::size_t buffer_size) : _allocated_buffer_size(buffer_size) {}
    
    /**
     * Allocates memory of size `size` with alignment `alignment`
     * @param size size of memory to allocate
     * @param alignment 
     */
    void* allocate(std::size_t size, std::size_t alignment) {
        return static_cast<AllocatorImpl*>(this)->allocate_impl(size, alignment);
    }
    /**
     * Deallocates memory given an address
     * @param ptr pointer to object to deallocate
     */
    void deallocate(void* ptr, size_t size) {
        return static_cast<AllocatorImpl*>(this)->deallocate_impl(ptr, size);
    }
    /**
     * Wipes current buffer clean, provides a newly allocated buffer with size _allocated_buffer_size
     */
    void reset() {
        return static_cast<AllocatorImpl*>(this)->reset_impl();
    }

    // no copy, no move
    allocator(const allocator& other) = delete;
    allocator& operator=(const allocator& other) = delete;
    allocator(const allocator&& other) = delete; 
    allocator& operator=(const allocator&& other) = delete; 

protected:
    std::size_t _allocated_buffer_size;
    std::size_t _used_amount{0};

};

class linear_allocator : public allocator<linear_allocator> {
public: 
    explicit linear_allocator(std::size_t buffer_size) : allocator<linear_allocator>(buffer_size) {
        _buffer_start_ptr = std::malloc(_allocated_buffer_size);
        _buffer_offset_ptr = _buffer_start_ptr;
    }

    ~linear_allocator() { std::free(_buffer_start_ptr); }

    void* allocate_impl(std::size_t allocation_size, std::size_t alignment) {
        std::size_t remaining_size = _allocated_buffer_size - _used_amount;
        // pointer to first aligned position in memory (to `alignment`) 
        void* aligned_ptr = std::align(alignment, allocation_size, _buffer_offset_ptr, remaining_size); 

        if (aligned_ptr != nullptr) {
            // calculate cost of alignment to update _used_amount
            std::size_t alignment_size = static_cast<char*>(aligned_ptr) - static_cast<char*>(_buffer_offset_ptr);
            _used_amount += alignment_size + allocation_size;
            _buffer_offset_ptr = static_cast<void*>(static_cast<char*>(aligned_ptr) + allocation_size);
        }

        return aligned_ptr;
    }
    void deallocate_impl(void* ptr, std::size_t) = delete;
    void reset_impl() {
        std::free(_buffer_start_ptr);
        _buffer_start_ptr = std::malloc(_allocated_buffer_size);
        _used_amount = 0;
    }

    // no copy, no move
    linear_allocator(const allocator& other) = delete;
    linear_allocator& operator=(const allocator& other) = delete; 
    linear_allocator(const allocator&& other) = delete;
    linear_allocator& operator=(const allocator&& other) = delete; 

private:
    void* _buffer_offset_ptr{nullptr}; // pointer to next available location in the buffer
    void* _buffer_start_ptr{nullptr};
};

template <typename T, std::size_t Alignment>
struct aligned_allocator {
    using value_type = T;

    template <typename U>
    struct rebind {
        using other = aligned_allocator<U, Alignment>;
    };

    aligned_allocator() noexcept = default;

    template <typename U>
    aligned_allocator(const aligned_allocator<U, Alignment>&) noexcept {}

    T* allocate(std::size_t n) {
        void* ptr = nullptr;
        if (posix_memalign(&ptr, Alignment, n * sizeof(T)) != 0) {
            throw std::bad_alloc();
        }
        return reinterpret_cast<T*>(ptr);
    }

    void deallocate(T* ptr, std::size_t) noexcept { free(ptr); }

    template <typename U>
    bool operator==(const aligned_allocator<U, Alignment>&) const noexcept { return true; }

    template <typename U>
    bool operator!=(const aligned_allocator<U, Alignment>&) const noexcept { return false; }
};

} // namespace collision engine 


