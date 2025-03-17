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

    // Mutex to control entry for readers, allowing writers to block new readers
    std::mutex reader_entry_mutex_;
    int waiting_writers_ = 0; // Track waiting writers for potential priority

public:
    // Acquire read lock
    void acquireReadLock() {
        std::unique_lock<std::mutex> reader_lock(reader_entry_mutex_); // Acquire reader entry lock
        std::unique_lock<std::mutex> lock(mutex_);
        reader_cv_.wait(lock, [this]{ return !writer_active_ && waiting_writers_ == 0; });
        reader_count_++;
    }

    // Release read lock
    void releaseReadLock() {
        std::unique_lock<std::mutex> lock(mutex_);
        reader_count_--;
        if (reader_count_ == 0) {
            writer_cv_.notify_one();
        }
    }

    // Acquire write lock
    void acquireWriteLock() {
        std::unique_lock<std::mutex> writer_lock(reader_entry_mutex_); // Block new readers
        std::unique_lock<std::mutex> lock(mutex_);
        waiting_writers_++;
        writer_cv_.wait(lock, [this]{ return reader_count_ == 0 && !writer_active_; });
        waiting_writers_--;
        writer_active_ = true;
    }

    // Release write lock
    void releaseWriteLock() {
        std::unique_lock<std::mutex> lock(mutex_);
        writer_active_ = false;
        writer_cv_.notify_one(); // Give priority to a waiting writer
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
        std::cout << "Writer " <
