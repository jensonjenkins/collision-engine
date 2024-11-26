#pragma once

#include <memory>
#include <mutex>
#include <queue>

namespace collision_engine {

template <typename T>
class thread_safe_queue {
private:
    mutable std::mutex mut;
    std::queue<T> data_queue;
    std::condition_variable data_cond;

public:
    thread_safe_queue() {}
    thread_safe_queue(const thread_safe_queue&) = delete;
    thread_safe_queue& operator=(const thread_safe_queue&) = delete;

    void push(T new_value) {
        std::lock_guard<std::mutex> lock(mut);
        data_queue.push(std::move(new_value));
        data_cond.notify_one();
    }
    
    /** 
     * @brief Waits for the queue to be non-empty before obtaining the front() value.
     * Idea behind passing the popped value as reference (i.e. T& value or std::shared_ptr<T>)    
     * is to avoid the use of copy constructors for T that may cause exceptions (e.g. std::bad_alloc).
     */
    void wait_and_pop(T& value) {
        std::unique_lock<std::mutex> lock(mut);
        data_cond.wait(lock, [this]{ return ! data_queue.empty(); });
        value = std::move(data_queue.front());
        data_queue.pop();
    }

    std::shared_ptr<T> wait_and_pop() {
        std::unique_lock<std::mutex> lock(mut);
        data_cond.wait(lock, [this]{ return ! data_queue.empty(); });
        std::shared_ptr<T> res(std::make_shared<T>(std::move(data_queue.front())));
        data_queue.pop();
        return res;
    }

    /** 
     * @brief Attempts to obtain the front() value if the queue is non-empty.
     * @param value reference to the storage of there the front() value is to be stored.
     * @return bool signifying whether a front() value is successfully obtained. 
     */
    bool try_pop(T& value) {
        std::lock_guard<std::mutex> lock(mut);
        if(data_queue.empty()) {
            return false;
        }
        value = std::move(data_queue.front());
        data_queue.pop();
        return true;
    }

    std::shared_ptr<T> try_pop() {
        std::lock_guard<std::mutex> lock(mut);
        if(data_queue.empty()) {
            return std::shared_ptr<T>();
        }
        std::shared_ptr<T> res(std::make_shared<T>(std::move(data_queue.front())));
        data_queue.pop();
        return res;
    }

    bool empty() const {
        std::lock_guard<std::mutex> lock(mut);
        return data_queue.empty();
    }
};

} // namespace collision_engine


