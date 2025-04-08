#include "timespec.h"

#include <time.h>
#include <errno.h>
#include <cmath>

const int NS_PER_SEC = 1000000000;
const int MS_PER_SEC = 1000;
const int NS_PER_MS = 1000000;

double timespec_to_ms(const timespec& time_ts) {
    return static_cast<double>(time_ts.tv_sec) * 1000.0 + static_cast<double>(time_ts.tv_nsec) / 1000000.0;
}

timespec timespec_from_ms(double time_ms){
    timespec time_ts;
    time_ts.tv_sec = static_cast<time_t>(time_ms / MS_PER_SEC);
    time_ts.tv_nsec = std::lround((time_ms - time_ts.tv_sec * MS_PER_SEC) * NS_PER_MS);
    if (time_ts.tv_nsec >= NS_PER_SEC) {
        time_ts.tv_sec += time_ts.tv_nsec / NS_PER_SEC;
        time_ts.tv_nsec %= NS_PER_SEC;
    }

    return time_ts;
}

timespec timespec_now() {
    timespec ts;
    clock_gettime(CLOCK_REALTIME, &ts);
    return ts;
}

timespec timespec_negate(const timespec& time_ts) {
    timespec ts_neg;
    ts_neg.tv_sec = -time_ts.tv_sec;
    
    if (time_ts.tv_nsec == 0) {
        ts_neg.tv_nsec = 0;
    } else {
        ts_neg.tv_nsec = (time_ts.tv_nsec == 0) ? 0 : 1000000000 - time_ts.tv_nsec;
        ts_neg.tv_sec -= 1;
    }

    return ts_neg;
}

timespec timespec_add(const timespec& time1_ts, const timespec& time2_ts) {
    timespec ts_sum;

    ts_sum.tv_sec = time1_ts.tv_sec + time2_ts.tv_sec;
    ts_sum.tv_nsec = time1_ts.tv_nsec + time2_ts.tv_nsec;

    if (ts_sum.tv_nsec >= 1000000000) {
        ts_sum.tv_sec += ts_sum.tv_nsec / NS_PER_SEC;
        ts_sum.tv_nsec %= NS_PER_SEC;
    }

    return ts_sum;

}

timespec timespec_subtrac(const timespec& time1_ts, const timespec& time2_ts) {
    timespec ts_diff;

    ts_diff.tv_sec = time1_ts.tv_sec - time2_ts.tv_sec;
    ts_diff.tv_nsec = time1_ts.tv_nsec - time2_ts.tv_nsec;

    if (ts_diff.tv_nsec < 0) {
        ts_diff.tv_sec -= 1;
        ts_diff.tv_nsec += NS_PER_SEC;
    }

    return ts_diff;
}


void timespec_wait(const timespec& delay_ts) {
    timespec remaining_time = delay_ts;

    while (remaining_time.tv_sec > 0 || remaining_time.tv_nsec > 0) {
        int ret = nanosleep(&remaining_time, &remaining_time);
        if (ret == -1) {
            if (errno == EINTR) {
                continue;
            } else {
                break;
	    }
        }
    break;
    }
}

timespec operator- (const timespec& time_ts) {
    return timespec_negate(time_ts);
}

timespec  operator+ (const timespec& time1_ts, const timespec& time2_ts){
    return timespec_add(time1_ts, time2_ts);
}

timespec  operator- (const timespec& time1_ts, const timespec& time2_ts){
    return timespec_subtrac(time1_ts, time2_ts);
}

timespec& operator+= (timespec& time_ts, const timespec& delay_ts){
    time_ts = timespec_add(time_ts, delay_ts);
    return time_ts;
}

timespec& operator-= (timespec& time_ts, const timespec& delay_ts){
    time_ts = timespec_subtrac(time_ts, delay_ts);
    return time_ts;
}

bool operator== (const timespec& time1_ts, const timespec& time2_ts){
    return (time1_ts.tv_sec == time2_ts.tv_sec) && (time1_ts.tv_nsec == time2_ts.tv_nsec);
}

bool operator!= (const timespec& time1_ts, const timespec& time2_ts){
    return (time1_ts.tv_sec != time2_ts.tv_sec) || (time1_ts.tv_nsec != time2_ts.tv_nsec);
}
bool operator< (const timespec& time1_ts, const timespec& time2_ts){
    if (time1_ts.tv_sec < time2_ts.tv_sec) return true;
    if (time1_ts.tv_sec > time2_ts.tv_sec) return false;
    return time1_ts.tv_nsec < time2_ts.tv_nsec;
}
bool operator> (const timespec& time1_ts, const timespec& time2_ts){
    if (time1_ts.tv_sec > time2_ts.tv_sec) return true;
    if (time1_ts.tv_sec < time2_ts.tv_sec) return false;
    return time1_ts.tv_nsec > time2_ts.tv_nsec;
}
