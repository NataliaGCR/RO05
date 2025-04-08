#ifndef TIMER_H
#define TIMER_H

#include <csignal>
#include <ctime>

class Timer {
protected:
    timer_t timerId;
    bool isRunning;

public:
    Timer();
    virtual ~Timer();

    void start(timespec duration, bool periodic);
    void start_ms(double milliseconds, bool periodic);
    void stop();

    virtual void callback() = 0; 

    static void call_callback(int signo, siginfo_t* info, void* context);
    static timespec timespec_from_ms(double milliseconds);
};

#endif