#pragma once

#include <thread>
#include <atomic>

namespace collision_engine {

class thread_safe_task_queue {
private:
    mutable std::mutex                  _mut;
    std::queue<std::function<void()>>   _data_queue;
    std::condition_variable             _data_cond;
    std::atomic_int32_t                 _remaining_tasks = 0;

public:
    thread_safe_task_queue() {}
    thread_safe_task_queue(const thread_safe_task_queue&) = delete;
    thread_safe_task_queue& operator=(const thread_safe_task_queue&) = delete;
    
    template <typename FunctionType>
    void push(FunctionType&& callback) {
        std::lock_guard<std::mutex> lock(_mut);
        _data_queue.push(std::move(callback));
        _data_cond.notify_one();
        _remaining_tasks++;
    }
    
    /** 
     * @brief Waits for the queue to be non-empty before obtaining the front() value.
     * Idea behind passing the popped value as reference (i.e. T& value or std::shared_ptr<T>)    
     * is to avoid the use of copy constructors for T that may cause exceptions (e.g. std::bad_alloc).
     */
    void wait_and_pop(std::function<void()>& callback) {
        std::unique_lock<std::mutex> lock(_mut);
        _data_cond.wait(lock, [this]{ return ! _data_queue.empty(); });
        callback = std::move(_data_queue.front());
        _data_queue.pop();
    }

    /** 
     * @brief Attempts to obtain the front() value if the queue is non-empty.
     * @param value reference to the storage of there the front() value is to be stored.
     * @return bool signifying whether a front() value is successfully obtained. 
     */
    bool try_pop(std::function<void()>& value) {
        std::lock_guard<std::mutex> lock(_mut);
        if(_data_queue.empty()) {
            return false;
        }
        value = std::move(_data_queue.front());
        _data_queue.pop();
        return true;
    }

    bool empty() const {
        std::lock_guard<std::mutex> lock(_mut);
        return _data_queue.empty();
    }
    
    static void wait() { std::this_thread::yield(); }

    void wait_task_completion() const {
        while (_remaining_tasks > 0) { 
            wait(); 
        }  
    }

    void work_done() { _remaining_tasks--; }
};

/**
 * @brief RAII class (to call .join()) for a group of threads when it goes out of scope
 */
class join_threads {
private:
    std::vector<std::thread>& _threads;
public:
    explicit join_threads(std::vector<std::thread>& threads) : _threads(threads) {};

    ~join_threads() { join_all_threads(); }
    
    void join_all_threads() {
        for (uint32_t i = 0; i < _threads.size(); i++) {
            if(_threads[i].joinable()) {
                _threads[i].join();
            }
        }
    }
};

class thread_pool {
private:
    join_threads                _joiner;
    std::atomic_bool            _done;
    std::vector<std::thread>    _threads;
    thread_safe_task_queue      _queue;
    
    void worker_thread() {
        while (!_done) {
            std::function<void()> task;
            if (_queue.try_pop(task)) {
                task();
                _queue.work_done();
            } else {
                std::this_thread::yield();
            }
        }
    }

public:
    const uint32_t thread_count = std::thread::hardware_concurrency();
    // const uint32_t thread_count = 2;

    thread_pool() : _done(false), _joiner(_threads) {
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
    void submit(FunctionType&& f) { _queue.push(std::forward<FunctionType>(f)); }

    void wait_for_tasks() const { _queue.wait_task_completion(); }
};

} // namespace collision_engine


