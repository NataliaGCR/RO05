// arm-linux-g++ -Wall -Wextra Looper.cpp td2c.cpp Chrono.cpp  timespec.cpp -o tp2c
#include <iostream>
#include <cstdlib>
#include <iomanip>
#include <vector>
#include "Looper.h"
#include "Chrono.h"

void runTest(unsigned int nLoops) {
    Looper looper;
    Chrono chrono;

    chrono.restart();
    double finalCount = looper.runLoop(nLoops);
    chrono.stop();

    std::cout << "nLoops: " << std::setw(12) << nLoops << " | "
              << "Valeur finale: " << std::setw(12) << finalCount << " | "
              << "Temps: " << std::fixed << std::setprecision(6) 
              << (chrono.lap_ms() / 1000.0) << " s" << std::endl;
}

int main() {
    std::vector<unsigned int> testValues = {
        100,
        1'000,
        10'000,
        100'000,
        1'000'000,
        10'000'000,
        100'000'000,
        500'000'000,
        750'000'000,
        1'000'000'000
    };

    std::cout << "Début des tests de performance..." << std::endl;
    std::cout << "----------------------------------------" << std::endl;
    std::cout << "     nLoops | Valeur finale | Temps d'exécution" << std::endl;
    std::cout << "----------------------------------------" << std::endl;

    for (auto nLoops : testValues) {
        runTest(nLoops);
    }

    std::cout << "----------------------------------------" << std::endl;
    std::cout << "Tests terminés." << std::endl;

    return 0;
}