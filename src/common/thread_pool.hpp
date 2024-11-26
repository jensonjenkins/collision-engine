#pragma once

#include "thread_safe_queue.hpp"
#include <thread>
#include <atomic>

namespace collision_engine {

class join_threads {
private:
    std::vector<std::thread>& _threads;
public:
    explicit join_threads(std::vector<std::thread>& threads) : _threads(threads) {};

    ~join_threads() {
        for (uint32_t i = 0; i < _threads.size(); i++) {
            if(_threads[i].joinable()) {
                _threads[i].join();
            }
        }
    }
};
class thread_pool {
private:
    join_threads                                _joiner;
    std::atomic_bool                            _done;
    std::vector<std::thread>                    _threads;
    thread_safe_queue<std::function<void()>>    _work_queue;
    
    void worker_thread() {
        while (!_done) {
            std::function<void()> task;
            if (_work_queue.try_pop(task)) {
                task();
            } else {
                std::this_thread::yield();
            }
        }
    }

public:
    thread_pool() : _done(false), _joiner(_threads) {
        const uint32_t thread_count = std::thread::hardware_concurrency();
        try {
            for (uint32_t i = 0; i < thread_count; i++) {
                _threads.push_back(std::thread(&thread_pool::worker_thread, this));
            }            
        } catch (...) {
            _done = true;
            throw;
        }
    }

    ~thread_pool() { _done = true; }

    template <typename FunctionType>
    void submit(FunctionType f) { _work_queue.push(std::function<void()>(f)); }
};

} // namespace collision_engine
