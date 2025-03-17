#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>

class UberRide {
private:
    int democratsWaiting = 0;
    int republicansWaiting = 0;
    int seatedCount = 0;  // Tracks how many riders are seated for the current ride
    std::mutex mtx;
    std::condition_variable cv;

public:
    void seatDemocrat() {
        std::unique_lock<std::mutex> lock(mtx);
        democratsWaiting++;

        // Wait until a valid ride configuration is possible
        cv.wait(lock, [this]() {
            return (democratsWaiting >= 4) || (democratsWaiting >= 2 && republicansWaiting >= 2);
        });

        // Form a valid ride
        if (democratsWaiting >= 4) {
            democratsWaiting -= 4;
        } else {
            democratsWaiting -= 2;
            republicansWaiting -= 2;
        }

        seated();
        seatedCount++;

        // Once 4 riders are seated, start the ride
        if (seatedCount == 4) {
            drive();
            seatedCount = 0;  // Reset for the next ride
        }

        cv.notify_all();
    }

    void seatRepublican() {
        std::unique_lock<std::mutex> lock(mtx);
        republicansWaiting++;

        // Wait until a valid ride configuration is possible
        cv.wait(lock, [this]() {
            return (republicansWaiting >= 4) || (republicansWaiting >= 2 && democratsWaiting >= 2);
        });

        // Form a valid ride
        if (republicansWaiting >= 4) {
            republicansWaiting -= 4;
        } else {
            republicansWaiting -= 2;
            democratsWaiting -= 2;
        }

        seated();
        seatedCount++;

        // Once 4 riders are seated, start the ride
        if (seatedCount == 4) {
            drive();
            seatedCount = 0;  // Reset for the next ride
        }

        cv.notify_all();
    }

    void seated() {
        std::cout << "A rider is seated.\n";
    }

    void drive() {
        std::cout << "The ride is starting.\n";
    }
};

void democrat(UberRide &uber) {
    uber.seatDemocrat();
}

void republican(UberRide &uber) {
    uber.seatRepublican();
}

int main() {
    UberRide uber;
    std::thread riders[8];

    // Create 4 Democrats and 4 Republicans
    for (int i = 0; i < 4; i++) {
        riders[i] = std::thread(democrat, std::ref(uber));
    }
    for (int i = 4; i < 8; i++) {
        riders[i] = std::thread(republican, std::ref(uber));
    }

    // Join threads
    for (int i = 0; i < 8; i++) {
        riders[i].join();
    }

    return 0;
}
