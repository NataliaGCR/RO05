#ifndef LOOPER_H
#define LOOPER_H

#include <atomic>

class Looper {
private:
    volatile std::atomic<bool> doStop;      
    volatile double iLoop; 

public:
    Looper();

    double runLoop(double nLoops);

    double stopLoop();

    double getSample() const;
};

#endif