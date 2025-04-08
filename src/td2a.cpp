//arm-linux-g++ -Wall -Wextra -Wno-psabi td2a.cpp Chrono.cpp timespec.cpp -o tp2a
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <time.h>
#include <string.h>
#include <unistd.h>

#define PERIOD_NS 500000000  // 0.5 sec en nanosecondes (2 Hz)
#define MAX_COUNT 15       


void myHandler(int, siginfo_t *si, void *) {
    auto *pCounter = (int *)si->si_value.sival_ptr;
    (*pCounter)++;
    printf("Tick %d\n", *pCounter);
}

int main(){
    struct sigaction sa;
    struct sigevent sev;
    struct itimerspec its;
    timer_t tid;

    int counter = 0;  

    memset(&sa, 0, sizeof(sa));
    sa.sa_flags = SA_SIGINFO;
    sa.sa_sigaction = myHandler;
    sigemptyset(&sa.sa_mask);
    sigaction(SIGRTMIN, &sa, nullptr);

    sev.sigev_notify = SIGEV_SIGNAL;
    sev.sigev_signo = SIGRTMIN;
    sev.sigev_value.sival_ptr = (void*) &counter;

    if (timer_create(CLOCK_REALTIME, &sev, &tid) == -1) {
        perror("timer_create");
        exit(EXIT_FAILURE);
    }

    its.it_value.tv_sec = 0;
    its.it_value.tv_nsec = PERIOD_NS;
    its.it_interval.tv_sec = 0;
    its.it_interval.tv_nsec = PERIOD_NS;

    if (timer_settime(tid, 0, &its, nullptr) == -1) {
        perror("timer_settime");
        exit(EXIT_FAILURE);
    }

    while(counter < MAX_COUNT){
    }

    timer_delete(tid);

    return 0;
}