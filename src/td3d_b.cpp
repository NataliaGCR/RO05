//arm-linux-g++ -Wall -Wextra -Wno-psabi  td3d_b.cpp Thread.cpp timespec.cpp Chrono.cpp Counter.cpp Incrementer.cpp -o tp3d_b
#include <iostream>
#include <vector>
#include <memory>
#include <cstring>
#include <sched.h>  // Para SCHED_RR
#include "Incrementer.h"
#include "Counter.h"
#include "Chrono.h"
#include "Thread.h"

int main(int argc, char* argv[]) {
    
    if (argc > 1 && std::strcmp(argv[1], "SCHED_RR") == 0) {
        Thread::setMainSched(SCHED_RR);
        std::cout << "Politique d'ordonnancement : SCHED_RR" << std::endl;
    } else {
        std::cout << "Politique d'ordonnancement : SCHED_OTHER (par défaut)" << std::endl;
    }
    

    std::vector<int> loopValues = {1000, 10000, 100000, 1000000, 10000000, 100000000};
    std::vector<int> taskValues = {1, 2, 3, 4, 5};

    for (int nLoops : loopValues) {
        for (int nTasks : taskValues) {
            std::cout << "Test avec nLoops = " << nLoops << " et nTasks = " << nTasks << std::endl;
            
            Counter counter(false);
            Chrono chrono;
            std::vector<std::unique_ptr<Incrementer>> incrementers;

            for (int i = 0; i < nTasks; i++) {
                incrementers.push_back(std::make_unique<Incrementer>(i, counter, nLoops));
            }

            chrono.restart();
            for (auto& inc : incrementers) {
                inc->start();
            }

            for (auto& inc : incrementers) {
                inc->join();
            }

            double elapsed = chrono.lap_ms();
            double expected = nLoops * nTasks;
            double actual = counter.value();
            double error = 100.0 * (expected - actual) / expected;

            std::cout << "Valeur attendue: " << expected << ", Valeur obtenue: " << actual << std::endl;
            std::cout << "Erreur: " << error << "%" << std::endl;
            std::cout << "Temps d'exécution: " << elapsed << "ms" << std::endl;
            std::cout << "----------------------------------------" << std::endl;
        }
    }

    return 0;
}
