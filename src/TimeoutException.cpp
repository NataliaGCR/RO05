#include "TimeoutException.h"

TimeoutException::TimeoutException(long timeout_ms) : timeout_ms(timeout_ms) {
    message = "Timeout after " + std::to_string(timeout_ms) + " ms";
}

const char* TimeoutException::what() const noexcept {
    return message.c_str();
}