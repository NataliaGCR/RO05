//  arm-linux-g++ -Wall -Wextra -Wno-psabi td3a_b.cpp -o tp3a_b
#include <pthread.h>
#include <iostream>
#include <cmath>
#include <unistd.h>
#include <stdint.h>
#include <time.h>
#include <iomanip> 

struct Data {
    volatile bool stop;
    volatile int64_t counter;
};

void* incrementer(void* v_data) {
    Data* p_data = (Data*)v_data;
    while (!p_data->stop) {
        __sync_fetch_and_add(&p_data->counter, 1);
    }
    return nullptr;
}

void run_test(int test_num) {
    Data data = {false, 0};
    pthread_t threads[3];
    const double target_duration = 3.0;

    struct timespec start, end;
    clock_gettime(CLOCK_MONOTONIC, &start);

    for (int i = 0; i < 3; ++i) {
        pthread_create(&threads[i], nullptr, incrementer, &data);
    }

    do {
        clock_gettime(CLOCK_MONOTONIC, &end);
    } while ((end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9 < target_duration);

    data.stop = true;

    for (int i = 0; i < 3; ++i) {
        pthread_join(threads[i], nullptr);
    }

    double duration = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;
    double mops = (data.counter / 1e6) / duration;

    std::cout << "Test #" << test_num 
              << " | Compteur: " << std::scientific << std::setprecision(2) << (double)data.counter
              << " | Durée: " << std::fixed << std::setprecision(3) << duration << "s"
              << " | Mops/s: " << std::fixed << std::setprecision(1) << mops << std::endl;
}

int main() {
    const int num_tests = 5;
    
    std::cout << "Début des tests avec 3 threads (3 secondes par test)\n";
    std::cout << "------------------------------------\n";

    for (int i = 1; i <= num_tests; ++i) {
        run_test(i);
    }

    std::cout << "------------------------------------\n";
    std::cout << "Tests terminés" << std::endl;
    
    return 0;
}