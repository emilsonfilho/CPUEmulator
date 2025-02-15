#ifndef OUT_OF_MEMORY_EXCEPTION_HPP
#define OUT_OF_MEMORY_EXCEPTION_HPP

#include <stdexcept>

using namespace std;

class OutOfMemoryException : public out_of_range {
public:
    explicit OutOfMemoryException();
};

#endif