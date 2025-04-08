#ifndef COUNTER_H
#define COUNTER_H

#include <memory>
#include <mutex>
#include <functional>

class Counter {
private:
    double m_value;
    std::unique_ptr<std::mutex> p_mutex;
    std::function<double()> incrementFunction;

    double incrProtected();
    double incrUnprotected();

public:
    Counter(bool protect);
    double increment();
    double value() const;
    bool isProtected() const;
};

#endif // COUNTER_H