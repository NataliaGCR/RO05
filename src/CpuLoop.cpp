#include "CpuLoop.h"
#include "Calibrator.h"

CpuLoop::CpuLoop(Calibrator& calibrator) : Looper(), calibrator(calibrator){}

void CpuLoop::runTime(double duration_ms)
{
    runLoop(calibrator.nLoops(duration_ms));
}
