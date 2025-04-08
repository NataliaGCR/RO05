#ifndef CALIBRATOR_H
#define CALIBRATOR_H

#include <vector>
#include <iostream>
#include <cmath>
#include <climits>
#include "Timer.h"
#include "Looper.h"
#include <utility>


class Calibrator : public Timer {
    public:
        Calibrator(double samplingPeriod_ms, unsigned int nSamples);
    
        void callback();
        double nLoops(double t);
    
    private:
        void estimateParameters();
    
        double samplingPeriod_ms;
        unsigned int nSamples;
        double a, b;
        std::vector<double> samples;
        Looper looper;
    };
    
    #endif // CALIBRATOR_H