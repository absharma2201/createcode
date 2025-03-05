#include <iostream>
#include <vector>
#include <shared_mutex>
#include <condition_variable>
#include <queue>
#include <thread>
#include <chrono>

template <typename T>
class ThreadSafeQueue {
private:
    std::queue<T> dataQueue;
    mutable std::shared_mutex mutex;  // Allows multiple readers, single writer
    std::condition_variable_any notEmpty;
    std::condition_variable_any notFull;
    size_t maxSize;

public:
    explicit ThreadSafeQueue(size_t maxSize) : maxSize(maxSize) {}

    void push(T value) {
        std::unique_lock<std::shared_mutex> lock(mutex);  // Exclusive lock for writing

        // Wait if the queue is full
        notFull.wait(lock, [this]() { return dataQueue.size() < maxSize; });

        dataQueue.push(value);
        std::cout << "Writer pushed: " << value << std::endl;

        // Notify readers that data is available
        notEmpty.notify_all();
    }

    T pop() {
        std::unique_lock<std::shared_mutex> lock(mutex);  // Exclusive lock for modification

        // Wait if the queue is empty
        notEmpty.wait(lock, [this]() { return !dataQueue.empty(); });

        T value = dataQueue.front();
        dataQueue.pop();
        std::cout << "Reader popped: " << value << std::endl;

        // Notify writers that space is available
        notFull.notify_all();

        return value;
    }

    size_t size() const {
        std::shared_lock<std::shared_mutex> lock(mutex);  // Allows multiple readers
        return dataQueue.size();
    }

    // New function: Read front element without removing it
    std::optional<T> peek() const {
        std::shared_lock<std::shared_mutex> lock(mutex);  // Multiple readers can check front
        if (dataQueue.empty()) return std::nullopt;
        return dataQueue.front();
    }
};

int main() {
    ThreadSafeQueue<int> queue(5); // Queue with a maximum size of 5

    // Writer thread
    std::thread writer([&]() {
        for (int i = 1; i <= 10; ++i) {
            queue.push(i);
            std::this_thread::sleep_for(std::chrono::milliseconds(500)); // Simulate write time
        }
    });

    // Reader threads
    std::vector<std::thread> readers;
    for (int i = 0; i < 3; ++i) { // 3 reader threads
        readers.emplace_back([&]() {
            for (int j = 0; j < 5; ++j) { // Each reader reads 5 times
                auto front = queue.peek();  // Check front without modifying
                if (front) {
                    std::cout << "Reader peeked: " << *front << std::endl;
                }
                int value = queue.pop();  // Remove the element
                std::this_thread::sleep_for(std::chrono::milliseconds(300)); // Simulate read time
            }
        });
    }

    // Join threads
    writer.join();
    for (auto& reader : readers) {
        reader.join();
    }

    std::cout << "All operations completed." << std::endl;

    return 0;
}
