#include "Looper.h"

Looper::Looper() : doStop(false), iLoop(0.0) {}

double Looper::runLoop(double nLoops) {
    iLoop = 0.0;
    doStop = false;
    while (iLoop < nLoops && !doStop) {
        iLoop += 1.0;
    }
    return iLoop;
}

double Looper::stopLoop() {
    doStop = true;
    return iLoop;
}

double Looper::getSample() const {
    return iLoop;
}