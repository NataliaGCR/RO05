#include "Semaphore.h"
#include <stdexcept>

Semaphore::Semaphore(CountType initValue, CountType maxValue) 
    : mutex(false), monitor(mutex), counter(initValue),  
      maxCount(maxValue == 0 ? std::numeric_limits<CountType>::max() : maxValue) {
    
    if (initValue > maxCount) {
        throw std::invalid_argument("initValue cannot be greater than maxValue");
    }
}

void Semaphore::give() {
    Monitor::Lock lock(monitor);
    if (counter < maxCount) {
        counter++;
        monitor.notifyAll();
    }
}

void Semaphore::take() {
    Monitor::Lock lock(monitor);
    while (counter == 0) {
        lock.wait();
    }
    counter--;
}

bool Semaphore::take(long timeout_ms) {
    Monitor::Lock lock(monitor);
    if (counter == 0) {
        if (!lock.wait(timeout_ms) || counter == 0) {
            return false;
        }
    }
    counter--;
    return true;
}
