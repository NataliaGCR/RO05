#ifndef COUNTDOWN_H
#define COUNTDOWN_H

#include "Timer.h"

class CountDown : public Timer {
    int count;
public:
    CountDown(int n);
    void callback() override;
};

#endif