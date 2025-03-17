#include <iostream>
#include <mutex>
#include <condition_variable>

class ReaderWriterLock {
private:
    std::mutex mutex_;
    std::condition_variable reader_cv_;
    std::condition_variable writer_cv_;
    int reader_count_ = 0;
    bool writer_active_ = false;

public:
    // Acquire read lock
    void acquireReadLock() {
        std::unique_lock<std::mutex> lock(mutex_);
        // Wait if a writer is active
        reader_cv_.wait(lock, [this]{ return !writer_active_; });
        reader_count_++;
    }

    // Release read lock
    void releaseReadLock() {
        std::unique_lock<std::mutex> lock(mutex_);
        reader_count_--;
        if (reader_count_ == 0) {
            // If no more readers, notify waiting writers
            writer_cv_.notify_one();
        }
    }

    // Acquire write lock
    void acquireWriteLock() {
        std::unique_lock<std::mutex> lock(mutex_);
        // Wait if any readers are active or a writer is active
        writer_cv_.wait(lock, [this]{ return reader_count_ == 0 && !writer_active_; });
        writer_active_ = true;
    }

    // Release write lock
    void releaseWriteLock() {
        std::unique_lock<std::mutex> lock(mutex_);
        writer_active_ = false;
        // Notify waiting readers and writers (writers get priority in this simple implementation)
        writer_cv_.notify_one();
        reader_cv_.notify_all();
    }
};

// Example usage:
#include <thread>
#include <vector>
#include <chrono>
#include <random>

ReaderWriterLock rw_lock;
int shared_data = 0;

void reader_function(int id) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> sleep_dist(10, 100);

    for (int i = 0; i < 5; ++i) {
        rw_lock.acquireReadLock();
        std::cout << "Reader " << id << " acquired read lock. Data: " << shared_data << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(sleep_dist(gen)));
        std::cout << "Reader " << id << " releasing read lock." << std::endl;
        rw_lock.releaseReadLock();
        std::this_thread::sleep_for(std::chrono::milliseconds(sleep_dist(gen)));
    }
}

void writer_function(int id) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> sleep_dist(50, 200);
    std::uniform_int_distribution<> data_dist(1, 100);

    for (int i = 0; i < 3; ++i) {
        rw_lock.acquireWriteLock();
        int new_data = data_dist(gen);
        std::cout << "Writer " << id << " acquired write lock. Writing data: " << new_data << std::endl;
        shared_data = new_data;
        std::this_thread::sleep_for(std::chrono::milliseconds(sleep_dist(gen)));
        std::cout << "Writer " << id << " releasing write lock." << std::endl;
        rw_lock.releaseWriteLock();
        std::this_thread::sleep_for(std::chrono::milliseconds(sleep_dist(gen)));
    }
}

int main() {
    std::vector<std::thread> threads;
    int num_readers = 5;
    int num_writers = 2;

    for (int i = 0; i < num_readers; ++i) {
        threads.emplace_back(reader_function, i + 1);
    }

    for (int i = 0; i < num_writers; ++i) {
        threads.emplace_back(writer_function, i + 1);
    }

    for (auto& thread : threads) {
        thread.join();
    }

    return 0;
}
