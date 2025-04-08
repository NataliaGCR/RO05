#include "Timer.h"
#include <iostream>
#include <csignal>
#include <ctime>

Timer::Timer() : isRunning(false) {
    struct sigaction sa;
    sa.sa_flags = SA_SIGINFO;
    sa.sa_sigaction = call_callback;
    sigemptyset(&sa.sa_mask);
    sigaction(SIGRTMIN, &sa, nullptr);

    struct sigevent sev;
    sev.sigev_notify = SIGEV_SIGNAL;
    sev.sigev_signo = SIGRTMIN;
    sev.sigev_value.sival_ptr = this; 
    if (timer_create(CLOCK_REALTIME, &sev, &timerId) == -1) {
        perror("timer_create");
        throw std::runtime_error("Timer creation failed");
    }
}

Timer::~Timer() {
    if (isRunning) {
        stop();
    }
    timer_delete(timerId);
}

void Timer::start(timespec duration, bool periodic) {
    if (isRunning) {
        stop();
    }

    struct itimerspec its;
    its.it_value = duration;
    its.it_interval = periodic ? duration : timespec{0, 0};

    if (timer_settime(timerId, 0, &its, NULL) == -1) {
        perror("timer_settime");
    }

    isRunning = true;
}

void Timer::start_ms(double milliseconds, bool periodic) {
    timespec duration = timespec_from_ms(milliseconds);
    start(duration, periodic);
}

void Timer::stop() {
    if (isRunning) {
        struct itimerspec its = {};
        timer_settime(timerId, 0, &its, NULL);
        isRunning = false;
    }
}

void Timer::call_callback(int signo [[maybe_unused]], siginfo_t* info, void* context [[maybe_unused]]) {
    Timer* timer = static_cast<Timer*>(info->si_value.sival_ptr);
    if (timer) {
        timer->callback();
    }
}

timespec Timer::timespec_from_ms(double milliseconds) {
    timespec ts;
    ts.tv_sec = static_cast<time_t>(milliseconds / 1000);
    ts.tv_nsec = static_cast<long>((milliseconds - ts.tv_sec * 1000) * 1e6);
    return ts;
}