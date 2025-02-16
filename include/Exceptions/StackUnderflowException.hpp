#ifndef STACK_UNDERFLOW_EXCEPTION_HPP
#define STACK_UNDERFLOW_EXCEPTION_HPP

#include <stdexcept>

class StackUnderflowException : public std::runtime_error {
public:
    explicit StackUnderflowException();
};

#endif