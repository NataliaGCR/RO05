#ifndef TIMEOUTEXCEPTION_H
#define TIMEOUTEXCEPTION_H

#include <stdexcept>
#include <string>

class TimeoutException : public std::exception {
private:
    long timeout_ms;
    std::string message;

public:
    TimeoutException(long timeout_ms);
    const char* what() const noexcept override;
};

#endif // TIMEOUTEXCEPTION_H