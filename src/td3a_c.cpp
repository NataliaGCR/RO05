// arm-linux-g++ -Wall -Wextra -Wno-psabi td3a_c.cpp -o tp3a_c
#include <pthread.h>
#include <iostream>
#include <chrono>
#include <iomanip>
#include <ctime>

struct Data {
    volatile bool stop;
    volatile double counter;
    pthread_mutex_t mutex;
};

void* incrementer(void* v_data) {
    Data* p_data = (Data*)v_data;
    while (!p_data->stop) {
        pthread_mutex_lock(&p_data->mutex);
        p_data->counter += 1.0;
        pthread_mutex_unlock(&p_data->mutex);
    }
    return nullptr;
}

void run_test(int test_num) {
    Data data = {false, 0.0, PTHREAD_MUTEX_INITIALIZER};
    pthread_t threads[3];
    
    std::cout << "Test #" << test_num << " en cours (3 secondes)..." << std::endl;

    for (int i = 0; i < 3; ++i) {
        pthread_create(&threads[i], nullptr, incrementer, &data);
    }

    timespec start, end;
    clock_gettime(CLOCK_MONOTONIC, &start);
    
    do {
        clock_gettime(CLOCK_MONOTONIC, &end);
    } while ((end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec)/1e9 < 3.0);

    data.stop = true;

    for (int i = 0; i < 3; ++i) {
        pthread_join(threads[i], nullptr);
    }

    double duration = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec)/1e9;
    double mops = (data.counter/1e6)/duration;

    std::cout << "Test #" << test_num 
              << " | Compteur: " << std::scientific << std::setprecision(2) << data.counter
              << " | Durée: " << std::fixed << std::setprecision(3) << duration << "s"
              << " | Mops/s: " << std::fixed << std::setprecision(1) << mops << std::endl;

    pthread_mutex_destroy(&data.mutex);
}

int main() {
    std::cout << "Début des tests avec mutex (3 threads, 3 secondes par test)\n";
    std::cout << "--------------------------------------------------------\n";

    const int num_tests = 5;
    for (int i = 1; i <= num_tests; ++i) {
        run_test(i);
    }

    std::cout << "--------------------------------------------------------\n";
    std::cout << "Tests terminés" << std::endl;

    return 0;
}