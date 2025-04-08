/* Monitor.h */
#ifndef MONITOR_H
#define MONITOR_H

#include "Mutex.h"
#include <pthread.h>

class Monitor {
private:
    pthread_cond_t posixCondId;
    Mutex& mutexRef;

public:
    explicit Monitor(Mutex& m);
    ~Monitor();

    void notify();
    void notifyAll();

    class Lock : public Mutex::Lock {
    private:
        Monitor& monitorRef;

    public:
        Lock(Monitor& monitor);
        Lock(Monitor& monitor, long timeout_ms);
        void wait();
        bool wait(long timeout_ms);
    };
};

#endif // MONITOR_H