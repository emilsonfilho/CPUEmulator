#include "../../include/Exceptions/StackOverflowException.hpp"

StackOverflowException::StackOverflowException() : runtime_error("Stack overflow!") {}