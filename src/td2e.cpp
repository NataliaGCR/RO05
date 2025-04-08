// arm-linux-g++ -Wall -Wextra -Wno-psabi td2e.cpp Looper.cpp CpuLoop.cpp Chrono.cpp Timer.cpp Calibrator.cpp timespec.cpp -o tp2e
#include "CpuLoop.h"
#include "Chrono.h"
#include "Calibrator.h"
#include <iostream>
#include <iomanip>
#include <cmath> // Pour fabs()


int main() {
    const double samplingPeriod_ms = 200.0;
    const unsigned int nSamples = 10;
    
    // Création du calibrateur
    Calibrator calibrator(samplingPeriod_ms, nSamples);
    CpuLoop cpuLoop(calibrator);

    std::cout << "Début des tests de précision..." << std::endl;
    std::cout << "----------------------------------------" << std::endl;
    std::cout << "Temps demandé | Temps mesuré | Erreur relative" << std::endl;
    std::cout << "----------------------------------------" << std::endl;

    // Tests de 0.5 à 10 secondes par pas de 200ms
    for (double targetTime_s = 0.5; targetTime_s <= 10.0; targetTime_s += 0.2) {
        Chrono chrono;
        chrono.restart();
        
        cpuLoop.runTime(targetTime_s * 1000.0); // Conversion en ms
        
        chrono.stop();
        
        double measuredTime_s = chrono.lap_ms() / 1000.0;
        double error = fabs((measuredTime_s - targetTime_s) / targetTime_s) * 100.0;

        std::cout << std::fixed << std::setprecision(2);
        std::cout << std::setw(11) << targetTime_s << " s  | " 
                  << std::setw(10) << std::setprecision(3) << measuredTime_s << " s | "
                  << std::setw(10) << std::setprecision(1) << error << "%" 
                  << (error > 10.0 ? " <-- ERREUR ÉLEVÉE!" : "") << std::endl;
    }

    std::cout << "----------------------------------------" << std::endl;
    std::cout << "Tests terminés." << std::endl;

    return 0;
}