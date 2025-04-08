#ifndef SEMAPHORE_H
#define SEMAPHORE_H

#include "Monitor.h"
#include <limits>

class Semaphore {
public:
    using CountType = unsigned int;

    Semaphore(CountType initValue, CountType maxValue = 0);
    void give();
    void take();
    bool take(long timeout_ms);

private:
    Mutex mutex;
    Monitor monitor;
    CountType counter;
    CountType maxCount;
};

#endif // SEMAPHORE_H
