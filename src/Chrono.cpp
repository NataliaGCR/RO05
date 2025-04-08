#include "Chrono.h"
#include "timespec.h"

Chrono::Chrono() { restart(); }

void Chrono::restart() {
    m_startTime = timespec_now();
    m_stopTime = m_startTime;
    active = true;
}

void Chrono::stop() {
    m_stopTime = timespec_now();
    active = false;
}

bool Chrono::isActive() const { return active; }

timespec Chrono::lap() const {
    return active ? timespec_subtrac(timespec_now(), m_startTime)
                  : timespec_subtrac(m_stopTime, m_startTime);
}

double Chrono::lap_ms() const {
    return timespec_to_ms(lap());
}
