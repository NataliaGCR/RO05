#include "Counter.h"

Counter::Counter(bool protect) : m_value(0.0) {
    if (protect) {
        p_mutex = std::make_unique<std::mutex>();
        incrementFunction = [this]() { return this->incrProtected(); };
    } else {
        incrementFunction = [this]() { return this->incrUnprotected(); };
    }
}

double Counter::incrProtected() {
    std::lock_guard<std::mutex> lock(*p_mutex);
    return ++m_value;
}

double Counter::incrUnprotected() {
    return ++m_value;
}

double Counter::increment() {
    return incrementFunction();
}

double Counter::value() const {
    return m_value;
}

bool Counter::isProtected() const {
    return p_mutex != nullptr;
}