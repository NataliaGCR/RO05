#include "Mutex.h"
#include <pthread.h>


Mutex::Mutex() : Mutex(false) { 
}

Mutex::Mutex(bool isInversionSafe) {
    pthread_mutexattr_init(&posixMutexAttrId);
    
    if (isInversionSafe) {
        pthread_mutexattr_settype(&posixMutexAttrId, PTHREAD_MUTEX_RECURSIVE);
        pthread_mutexattr_setprotocol(&posixMutexAttrId, PTHREAD_PRIO_INHERIT);
    } 

    if (pthread_mutex_init(&posixMutexId, &posixMutexAttrId) != 0) {
        throw std::runtime_error("Failed to initialize mutex");
    }

}

Mutex::~Mutex() {
    pthread_mutex_destroy(&posixMutexId);
    pthread_mutexattr_destroy(&posixMutexAttrId);
}

void Mutex::lock() {
    if (pthread_mutex_lock(&posixMutexId) != 0) {
        throw std::runtime_error("Failed to lock mutex");
    }
}

bool Mutex::lock(double timeout_ms) {
    timespec ts = timespec_now() + timespec_from_ms(timeout_ms);
    int result = pthread_mutex_timedlock(&posixMutexId, &ts);
    if (result == ETIMEDOUT) {
        return false;
    } else if (result != 0) {
        throw std::runtime_error("Failed to lock mutex with timeout");
    }
    return true;
}

void Mutex::unlock() {
    if (pthread_mutex_unlock(&posixMutexId) != 0) {
        throw std::runtime_error("Failed to unlock mutex");
    }
}

Mutex::Lock::Lock(Mutex& m) : mutex(m) {
    mutex.lock();
}


Mutex::Lock::Lock(Mutex& m, double timeout_ms) : mutex(m) {
    if (!mutex.lock(timeout_ms)) {
        throw TimeoutException(static_cast<long>(timeout_ms));
    }
}

Mutex::Lock::~Lock() {
    mutex.unlock();
}



