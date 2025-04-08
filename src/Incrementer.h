#ifndef INCREMENTER_H
#define INCREMENTER_H

#include "Thread.h"
#include "Counter.h"

class Incrementer : public Thread {
private:
    Counter& counter;
    const double nLoops;
public:
    Incrementer(int id, Counter& counter, double nLoops);
    void run() override;
};

#endif