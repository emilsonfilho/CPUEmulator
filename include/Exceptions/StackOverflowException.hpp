#ifndef STACK_OVERFLOW_EXCEPTION_HPP
#define STACK_OVERFLOW_EXCEPTION_HPP

#include <stdexcept>

class StackOverflowException : public std::runtime_error {
public:
    explicit StackOverflowException();
};

#endif