#ifndef THREAD_H
#define THREAD_H

#include <pthread.h>
#include <chrono>
#include "timespec.h"
#include "TimeoutException.h"

class Thread {


public:
    Thread(int id);
    virtual ~Thread();

    void start(int priority = 0, int policy = SCHED_OTHER);
    void join();
    virtual void run() = 0; 
    const int id;

    bool isStarted() const;
    long duration_ms() const;

    static int getMaxPrio(int policy);
    static int getMinPrio(int policy);
    static void setMainSched(int policy);
    static int getMainSched(int* policy = nullptr);
    void setPriority(int policy, int priority);

protected:
    pthread_t posixThreadId;
    pthread_attr_t posixThreadAttrId;
    
    bool started;
    std::chrono::steady_clock::time_point startTime;
    std::chrono::steady_clock::time_point endTime;

    static void* call_run(void* v_thread);

};

#endif // THREAD_H