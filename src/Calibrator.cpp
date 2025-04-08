#include "Calibrator.h"
#include "Chrono.h"
#include "timespec.h"
#include "Looper.h"
#include "Timer.h"
#include <cfloat> 
#include <iostream>


Calibrator::Calibrator(double samplingPeriod_ms, unsigned int nSamples)
    : samplingPeriod_ms(samplingPeriod_ms), nSamples(nSamples), a(0), b(0) {
    std::cout << "Inicializing calibrator constructor..." << std::endl;
    timespec samplingPeriod_ts = timespec_from_ms(samplingPeriod_ms);
    start(samplingPeriod_ts, true); 
    looper.runLoop(DBL_MAX); 
    stop();

    estimateParameters();
}

void Calibrator::callback() {
    double sample = looper.getSample();
    samples.push_back(sample);

    if (samples.size() == nSamples) {
        looper.stopLoop();
    }
}


void Calibrator::estimateParameters() {
    double sumX = 0, sumY = 0, sumXX = 0, sumXY = 0;
    for (unsigned i = 0; i < nSamples; ++i) {
        double x = times[i];   
        double y = samples[i];      

        sumX  += x;
        sumY  += y;
        sumXX += x * x;
        sumXY += x * y;
    }

    double n = static_cast<double>(nSamples);
    a = (n * sumXY - sumX * sumY) / (n * sumXX - sumX * sumX);
    b = (sumY - a * sumX) / n;

    std::cout << "Estimé a: " << a << ", b: " << b << std::endl;
}


double Calibrator::nLoops(double t) {
    return a * t + b; 
}