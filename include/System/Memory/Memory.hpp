#ifndef MEMORY_HPP
#define MEMORY_HPP

#include <vector>
#include <cstdint>

#include "../../Exceptions/OutOfMemoryException.hpp"

using namespace std;

class Memory {
    vector<uint8_t> memory; // 1-byte memory cell

    void validateAddress(uint16_t address);
public:
    static const size_t MEMORY_SIZE = 65536;

    Memory();
    uint16_t read(uint16_t address);
    void write(uint16_t address, uint16_t data);
};

#endif