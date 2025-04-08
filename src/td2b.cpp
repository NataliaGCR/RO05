//arm-linux-g++ -Wall -Wextra -Wno-psabi td2b.cpp Timer.cpp Chrono.cpp timespec.cpp CountDown.cpp -o tp2b
#include "CountDown.h"
#include "Timer.h"
#include <stdio.h>

int main() {
    CountDown cd(10); 
    cd.start_ms(1000, true); 

    while (true) {
        // Sleep or perform other tasks
    }

    return 0;
}