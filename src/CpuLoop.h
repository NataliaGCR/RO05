#ifndef CPULOOP_H
#define CPULOOP_H

#include "Calibrator.h"
#include "Looper.h"

class CpuLoop : public Looper {
public:
    CpuLoop(Calibrator& calibrator);

    void runTime(double duration_ms);

private:
    Calibrator& calibrator;
    Looper looper;
};

#endif // CPULOOP_H