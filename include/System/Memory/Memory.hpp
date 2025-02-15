#ifndef MEMORY_HPP
#define MEMORY_HPP

#include <vector>
#include <cstdint>
#include <string>
#include <sstream>
#include <iomanip>
#include <set>

#include "../../Exceptions/OutOfMemoryException.hpp"
#include "../../Utils/ApplyFormat.hpp"

using namespace std;

class Memory {
    vector<uint8_t> memory; // 1-byte memory cell
    set<uint16_t> indexesAccessed;

    void validateAddress(uint16_t address) const;

    string printMemory(uint16_t startAddress, uint16_t endAddress, bool isStackRange = false) const;
public:
    static const size_t MEMORY_SIZE = 65536;

    Memory();
    uint16_t read(uint16_t address) const;
    void write(uint16_t address, uint16_t data);

    string toString() const;
    string getStack(uint16_t SP) const;

    friend class CPU;
};

#endif