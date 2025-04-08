#ifndef MUTEX_H
#define MUTEX_H

#include <pthread.h>
#include <stdexcept>
#include <ctime>
#include <cerrno>
#include "timespec.h"
#include "TimeoutException.h"

timespec timespec_now();
timespec timespec_from_ms(double ms);

class Mutex {
    friend class Monitor;
public:
    Mutex();
    explicit Mutex(bool isInversionSafe = false);
    ~Mutex();

    void lock();
    bool lock(double timeout_ms);
    void unlock();
    
    class Lock {
    public:
        explicit Lock(Mutex& m);
        Lock(Mutex& m, double timeout_ms);
        ~Lock();

    private:
        Mutex& mutex;
    };

private:
    pthread_mutex_t posixMutexId;
    pthread_mutexattr_t posixMutexAttrId;
};

#endif // MUTEX_H   