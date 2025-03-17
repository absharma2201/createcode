#include <iostream>
#include <vector>
#include <thread>
#include <mutex>
#include <chrono>
#include <random>

class DiningPhilosophers {
private:
    int num_philosophers;
    std::vector<std::mutex> forks;
    bool stop_dining = false; // Stop flag

public:
    DiningPhilosophers(int n) : num_philosophers(n), forks(n) {}

    void philosopher(int id) {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> think_time(100, 500);
        std::uniform_int_distribution<> eat_time(200, 800);

        int left_fork = id;
        int right_fork = (id + 1) % num_philosophers;

        // To prevent deadlock, always pick up the lower-numbered fork first
        if (left_fork > right_fork) std::swap(left_fork, right_fork);

        while (!stop_dining) {
            // Thinking
            std::cout << "Philosopher " << id << " is thinking.\n";
            std::this_thread::sleep_for(std::chrono::milliseconds(think_time(gen)));

            // Pick up forks
            std::lock_guard<std::mutex> left_lock(forks[left_fork]);
            std::cout << "Philosopher " << id << " picked up fork " << left_fork << "\n";

            std::lock_guard<std::mutex> right_lock(forks[right_fork]);
            std::cout << "Philosopher " << id << " picked up fork " << right_fork << "\n";

            // Eating
            std::cout << "Philosopher " << id << " is eating.\n";
            std::this_thread::sleep_for(std::chrono::milliseconds(eat_time(gen)));

            // Automatically releases forks when locks go out of scope
        }
    }

    void start_dining() {
        std::vector<std::thread> threads;
        for (int i = 0; i < num_philosophers; ++i) {
            threads.emplace_back(&DiningPhilosophers::philosopher, this, i);
        }

        std::this_thread::sleep_for(std::chrono::seconds(20)); // Let them eat

        stop_dining = true; // Stop philosophers

        for (auto& t : threads) {
            if (t.joinable()) t.join(); // Clean termination
        }
    }
};

int main() {
    DiningPhilosophers dp(5);
    dp.start_dining();
    return 0;
}
