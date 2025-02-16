#include "../../include/Exceptions/StackUnderflowException.hpp"

StackUnderflowException::StackUnderflowException() : runtime_error("Stack underflow!") {}