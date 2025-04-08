#include "Thread.h"
#include <iostream>
#include <stdexcept>

void* Thread::call_run(void* v_thread) {
    Thread* thread = static_cast<Thread*>(v_thread);
    thread->run();
    thread->endTime = std::chrono::steady_clock::now();
    return nullptr;
}

Thread::Thread(int id) : id(id), started(false) {
    posixThreadId = 0;
    if (pthread_attr_init(&posixThreadAttrId) != 0) {
        throw std::runtime_error("Failed to initialize thread attributes");
    }
    if (pthread_attr_setschedpolicy(&posixThreadAttrId, SCHED_OTHER) != 0) {
        throw std::runtime_error("Failed to set thread scheduling policy");
    }
    if (pthread_attr_setinheritsched(&posixThreadAttrId, PTHREAD_EXPLICIT_SCHED) != 0) {
        throw std::runtime_error("Failed to set inherit sched");
    }
}

Thread::~Thread() {
    pthread_attr_destroy(&posixThreadAttrId);
}

void Thread::start(int priority, int policy) {
    if (!started) {
        this->startTime = std::chrono::steady_clock::now();

        int min_prio = getMinPrio(policy);
        int max_prio = getMaxPrio(policy);

        if (priority < min_prio) {
            priority = min_prio;
        } else if (priority > max_prio) {
            priority = max_prio;
        }

        sched_param schedParams;
        schedParams.sched_priority = priority;

        pthread_attr_setschedparam(&posixThreadAttrId, &schedParams);

        if (pthread_create(&posixThreadId, &posixThreadAttrId, &Thread::call_run, this) != 0) {
            throw std::runtime_error("Failed to create thread");
        }
        if (pthread_attr_setschedparam(&posixThreadAttrId, &schedParams) != 0) {
            throw std::runtime_error("Failed to set thread scheduling parameters");
        }
        started = true;
    }
}

void Thread::join() {
    if (started && pthread_join(posixThreadId, nullptr) == 0) {
        started = false;
    } else {
        throw std::runtime_error("Failed to join thread");
    }
}

bool Thread::isStarted() const {
    return started;
}

long Thread::duration_ms() const {
    if (!started) {
        throw std::runtime_error("Thread not started");
    }
    if (std::chrono::steady_clock::time_point{} != endTime) {
        return std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count();
    }
    throw std::runtime_error("Thread is still running");
}

int Thread::getMaxPrio(int policy) {
    return sched_get_priority_max(policy);
}

int Thread::getMinPrio(int policy) {
    return sched_get_priority_min(policy);
}

void Thread::setMainSched(int policy) {
    if (policy != SCHED_OTHER && policy != SCHED_FIFO && policy != SCHED_RR) {
        throw std::invalid_argument("Invalid scheduling policy");
    }
    struct sched_param param;
    param.sched_priority = (policy == SCHED_OTHER) ? 0 : getMaxPrio(policy);
    if (pthread_setschedparam(pthread_self(), policy, &param) != 0) {
        throw std::runtime_error("Failed to set main thread scheduling policy");
    }
}

int Thread::getMainSched(int* policy) {
    struct sched_param param;
    int currentPolicy;
    if (pthread_getschedparam(pthread_self(), &currentPolicy, &param) != 0) {
        throw std::runtime_error("Failed to get main thread scheduling policy");
    }
    if (policy) {
        *policy = currentPolicy;
    }
    return param.sched_priority;
}

void Thread::setPriority(int policy, int priority) {
    if (!started) {
        throw std::runtime_error("Thread must be started before setting priority");
    }

    sched_param schedParams;
    schedParams.sched_priority = priority;

    if (pthread_setschedparam(posixThreadId, policy, &schedParams) != 0) {
        throw std::runtime_error("Failed to set thread priority");
    }
}