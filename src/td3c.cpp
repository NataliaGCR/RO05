//arm-linux-g++ -Wall -Wextra -Wno-psabi td3b.cpp Mutex.cpp TimeoutException.cpp timespec.cpp Thread.cpp -o tp3c
#include "Thread.h"
#include "Mutex.h"
#include "TimeoutException.h"
#include <iostream>

class WorkerThread : public Thread {
private:
    Mutex mutex;

public:
    WorkerThread(int id) : Thread(id) {}

    void run() override {
        try {
            Mutex::Lock lock(mutex);
            std::cout << "Thread " << id << " is running." << std::endl;

        } catch (const TimeoutException& e) {
            std::cerr << "Timeout while locking mutex: " << e.what() << std::endl;
        } catch (const std::exception& e) {
            std::cerr << "Exception: " << e.what() << std::endl;
        }
    }
};

int main() {
    WorkerThread thread1(1);
    WorkerThread thread2(2);

    thread1.start();
    thread2.start();

    thread1.join();
    thread2.join();

    return 0;
}
