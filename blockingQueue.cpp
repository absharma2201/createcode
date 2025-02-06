#include <iostream>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>

class BoundedQueue {
private:
    std::queue<int> queue;
    std::mutex mtx;
    std::condition_variable cv_full;
    std::condition_variable cv_empty;
    size_t capacity;

public:
    BoundedQueue(size_t cap) : capacity(cap) {}

    void produce(int item) {
        std::unique_lock<std::mutex> lock(mtx);
        cv_full.wait(lock, [this]() { return queue.size() < capacity; });
        queue.push(item);
        std::cout << "Produced: " << item << std::endl;
        cv_empty.notify_one();
    }

    int consume() {
        std::unique_lock<std::mutex> lock(mtx);
        cv_empty.wait(lock, [this]() { return !queue.empty(); });
        int item = queue.front();
        queue.pop();
        std::cout << "Consumed: " << item << std::endl;
        cv_full.notify_one();
        return item;
    }
};

void producer(BoundedQueue& q, int count) {
    for (int i = 0; i < count; ++i) {
        q.produce(i);
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

void consumer(BoundedQueue& q, int count) {
    for (int i = 0; i < count; ++i) {
        q.consume();
        std::this_thread::sleep_for(std::chrono::milliseconds(150));
    }
}

int main() {
    BoundedQueue q(5);
    std::thread prodThread(producer, std::ref(q), 10);
    std::thread consThread(consumer, std::ref(q), 10);

    prodThread.join();
    consThread.join();

    return 0;
}
