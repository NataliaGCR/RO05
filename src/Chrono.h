#ifndef CHRONO_H
#define CHRONO_H

#include "timespec.h"
#include <ctime>

class Chrono {
private:
    timespec m_startTime;
    timespec m_stopTime;
    bool active;

public:
    Chrono();

    void restart();
    void stop();
    bool isActive() const;
    timespec lap() const;
    double lap_ms() const;
};

#endif // CHRONO_H
