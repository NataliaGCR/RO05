#include "CountDown.h"
#include <iostream>
#include "Timer.h"

CountDown::CountDown(int n) : count(n) {}

void CountDown::callback() {
    if (count >= 0) {
        std::cout << count << std::endl;
        --count;
    } else {
        stop();
    }
}
