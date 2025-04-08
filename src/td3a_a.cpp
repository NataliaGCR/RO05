//  arm-linux-g++ -Wall -Wextra -Wno-psabi td3a_a.cpp -o tp3a_a
#include <pthread.h>
#include <iostream>
#include <chrono>
#include <atomic>
#include <vector>
#include <numeric>
#include <iomanip>
#include <cmath> 
#include <unistd.h> 

std::atomic<bool> stop(false);

struct ThreadResult {
    double count;
    double duration;
};

void* incrementer(void* arg) {
    (void)arg; 
    double counter = 0.0;
    auto start = std::chrono::high_resolution_clock::now();
    
    while (!stop.load(std::memory_order_relaxed)) {
        counter += 1.0;
        asm volatile("" : "+r"(counter) : : "memory");
    }
    
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration<double>(end - start).count();
    
    ThreadResult* result = new ThreadResult{counter, duration};
    return result;
}

void runTest(int testNum) {
    stop = false;
    pthread_t thread;
    
    std::cout << "Test #" << testNum << " en cours..." << std::flush;
    
    pthread_create(&thread, nullptr, incrementer, nullptr);
    
    usleep(3000000);
    
    stop = true;
    
    ThreadResult* result;
    pthread_join(thread, (void**)&result);
    
    std::cout << "\rTest #" << testNum 
              << " | Compteur: " << std::scientific << std::setprecision(2) << result->count
              << " | Durée: " << std::fixed << std::setprecision(3) << result->duration << "s"
              << " | Mops/s: " << std::setprecision(1) << (result->count / 1e6 / result->duration)
              << std::endl;
    
    delete result;
}

int main() {
    const int NUM_TESTS = 10;
    std::vector<double> counts;
    
    std::cout << "Début des tests (3 secondes par test)\n";
    std::cout << "----------------------------------------\n";
    
    for (int i = 1; i <= NUM_TESTS; ++i) {
        runTest(i);
        if (i < NUM_TESTS) {
            usleep(500000); 
        }
    }
    
    std::cout << "----------------------------------------\n";
    
    if (!counts.empty()) {
        double average = std::accumulate(counts.begin(), counts.end(), 0.0) / counts.size();
        std::cout << "Moyenne: ~" << std::scientific << std::setprecision(2) << average 
                  << " opérations/3s\n";
        std::cout << "Ordre de grandeur: 10^" << (int)log10(average) << "\n";
    }
    
    return 0;
}