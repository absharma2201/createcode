#include <iostream>
#include <chrono>
#include <functional>
#include <thread>
#include <atomic>
#include <mutex>
#include <condition_variable>

class Timer {
private:
    std::function<void(void)> m_func;
    long m_interval;
    long m_timeout;
    std::thread m_thread;
    std::atomic<bool> m_running{false};
    std::mutex m_mutex;
    std::condition_variable m_cv;

public:
    Timer() = default;

    Timer(std::function<void(void)> func, long interval) 
        : m_func(std::move(func)), m_interval(interval) {}

    void start() {
        if (m_running.load()) return;  // Prevent restarting without stopping
        m_running = true;
        m_thread = std::thread([this]() {
            while (m_running.load()) {
                auto delta = std::chrono::steady_clock::now() + std::chrono::milliseconds(m_interval);
                m_func();
                std::unique_lock<std::mutex> lock(m_mutex);
                if (m_cv.wait_until(lock, delta, [this] { return !m_running.load(); })) break;
            }
        });
    }

    void startOnce() {
        if (m_running.load()) return;
        m_running = true;
        m_thread = std::thread([this]() {
            auto delta = std::chrono::steady_clock::now() + std::chrono::milliseconds(m_timeout);
            m_func();
            std::this_thread::sleep_until(delta);
            stop();
        });
    }

    void stop() {
        if (!m_running.load()) return;
        m_running = false;
        m_cv.notify_all();  // Wake up the thread to exit loop
        if (m_thread.joinable()) m_thread.join();
        std::cout << "Stopping Timer: " << m_interval << "ms\n";
    }

    Timer* setFunc(std::function<void(void)> func) {
        m_func = std::move(func);
        return this;
    }

    Timer* setInterval(long interval) {
        m_interval = interval;
        return this;
    }

    Timer* setTimeout(long timeout) {
        m_timeout = timeout;
        return this;
    }

    ~Timer() {
        stop();
    }
};

int main() {
    int iGoUp = 0, iGoDown = 20;
    Timer t1;

    t1.setFunc([&]() {
        if (++iGoUp == --iGoDown) {
            t1.stop();
        }
        std::cout << iGoUp << " - " << iGoDown << std::endl;
    })
    ->setInterval(1000)
    ->start();

    Timer t2;
    t2.setFunc([&]() {
        std::cout << "Starting Timer 2" << std::endl;
    })
    ->setTimeout(5000)
    ->startOnce();

    std::cin.get();
    return 0;
}
