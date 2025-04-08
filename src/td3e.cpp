// arm-linux-g++ -Wall -Wextra -Wno-psabi td3e.cpp Thread.cpp timespec.cpp CpuLoop.cpp Chrono.cpp Counter.cpp Incrementer.cpp Calibrator.cpp Mutex.cpp Timer.cpp Looper.cpp -o tp3e

#include "Thread.h"
#include "Mutex.h"
#include "Calibrator.h"
#include "CpuLoop.h"
#include <iostream>

Mutex resource(false);
Calibrator calibrator(1000, 5000);  // tics de 10ms * 5000 = 50s máx duración

class Thread_C : public Thread {
private:
    CpuLoop cpuLoop;
    
public:
    Thread_C(int id) : Thread(id), cpuLoop(calibrator) {}

    void run() override {
        std::cout << "[C] Starting at " << duration_ms() << " ms\n";
        cpuLoop.runTime(2000); // 200 tics (2000ms)

        std::cout << "[C] Waiting for resource at " << duration_ms() << " ms\n";
        resource.lock();
        std::cout << "[C] Locked resource at " << duration_ms() << " ms\n";

        cpuLoop.runTime(2000); // 200 tics with resource

        resource.unlock();
        std::cout << "[C] Released resource at " << duration_ms() << " ms\n";

        cpuLoop.runTime(1000); // 100 tics after
        std::cout << "[C] Finished at " << duration_ms() << " ms\n";
    }
};

class Thread_B : public Thread {
private:
    CpuLoop cpuLoop;
    
public:
    Thread_B(int id) : Thread(id), cpuLoop(calibrator) {}

    void run() override {
        timespec_wait(timespec_from_ms(3000));  // t = 3000 ms
        std::cout << "[B] Starting at " << duration_ms() << " ms\n";

        cpuLoop.runTime(1000); // 100 tics
        std::cout << "[B] Finished at " << duration_ms() << " ms\n";
    }
};

class Thread_A : public Thread {
private:
    CpuLoop cpuLoop;

public:
    Thread_A(int id) : Thread(id), cpuLoop(calibrator) {}

    void run() override {
        timespec_wait(timespec_from_ms(3000));  // t = 3000 ms
        std::cout << "[A] Starting at " << duration_ms() << " ms\n";

        cpuLoop.runTime(1000); // 100 tics before resource
        std::cout << "[A] Waiting for resource at " << duration_ms() << " ms\n";

        resource.lock();
        std::cout << "[A] Locked resource at " << duration_ms() << " ms\n";

        cpuLoop.runTime(1000); // 100 tics with resource
        resource.unlock();
        std::cout << "[A] Released resource at " << duration_ms() << " ms\n";

        cpuLoop.runTime(2000); // 200 tics after
        std::cout << "[A] Finished at " << duration_ms() << " ms\n";
    }
};

void runScenario(bool isProtected) {
    resource = Mutex(isProtected);

    Thread_C task_C(3); 
    Thread_B task_B(2);
    Thread_A task_A(1);

    std::cout << "=============================\n";
    std::cout << "Running scenario " << (isProtected ? "WITH" : "WITHOUT") << " priority inheritance\n";
    std::cout << "=============================\n";

    // Start C at t = 0
    task_C.start(3);

    // Start A and B at t = 3000ms
    timespec_wait(timespec_from_ms(3000));
    task_B.start(2);
    task_A.start(1);

    // Wait for all to finish
    task_C.join();
    task_B.join();
    task_A.join();

    std::cout << "-----------------------------\n";
    std::cout << "Results:\n";
    std::cout << "Task A elapsed time: " << task_A.duration_ms() << " ms\n";
    std::cout << "Task B elapsed time: " << task_B.duration_ms() << " ms\n";
    std::cout << "Task C elapsed time: " << task_C.duration_ms() << " ms\n";
    std::cout << "=============================\n\n";
}

int main() {
    runScenario(false);
    runScenario(true);
    return 0;
}
