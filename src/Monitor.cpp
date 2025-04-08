#include "Monitor.h"
#include <sys/time.h>
#include <errno.h>

Monitor::Monitor(Mutex& m) : mutexRef(m) {
    pthread_cond_init(&posixCondId, nullptr);
}

Monitor::~Monitor() {
    pthread_cond_destroy(&posixCondId);
}

void Monitor::notify() {
    pthread_cond_signal(&posixCondId);
}

void Monitor::notifyAll() {
    pthread_cond_broadcast(&posixCondId);
}

Monitor::Lock::Lock(Monitor& monitor)
    : Mutex::Lock(monitor.mutexRef), monitorRef(monitor) {}

Monitor::Lock::Lock(Monitor& monitor, long timeout_ms)
    : Mutex::Lock(monitor.mutexRef, timeout_ms), monitorRef(monitor) {}

void Monitor::Lock::wait() {
    pthread_cond_wait(&monitorRef.posixCondId, &monitorRef.mutexRef.posixMutexId);
}

bool Monitor::Lock::wait(long timeout_ms) {
    struct timespec ts;
    clock_gettime(CLOCK_REALTIME, &ts);
    ts.tv_sec += timeout_ms / 1000;
    ts.tv_nsec += (timeout_ms % 1000) * 1000000;
    if (ts.tv_nsec >= 1000000000) {
        ts.tv_sec++;
        ts.tv_nsec -= 1000000000;
    }
    return pthread_cond_timedwait(&monitorRef.posixCondId, &monitorRef.mutexRef.posixMutexId, &ts) == 0;
}
