// arm-linux-g++ -Wall -Wextra -Wno-psabi Looper.cpp td2d.cpp Chrono.cpp Timer.cpp Calibrator.cpp timespec.cpp -o tp2d
#include "Chrono.h"
#include "Calibrator.h"
#include <iostream>
#include <cfloat>

int main() {
    double samplingPeriod_ms = 200.0; 
    unsigned int nSamples = 10;

    Calibrator calibrator(samplingPeriod_ms, nSamples);

    for (unsigned i = 0; i < nSamples; ++i) {
        double loops = baseLoops * (i + 1);  
        Chrono chrono;
        chrono.restart();
    
        looper.runLoop(loops);
    
        chrono.stop();
        double elapsed = chrono.lap_ms();
        times.push_back(elapsed);
        samples.push_back(loops);
    }
    

    return 0;
}
