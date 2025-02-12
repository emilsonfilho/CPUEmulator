#include "../../include/System/Memory/Memory.hpp"

/* Private methods */
void Memory::validateAddress(uint16_t address) {
    if (address >= MEMORY_SIZE) 
        throw OutOfMemoryException();
}

/* Public methods */
Memory::Memory() {
    memory.resize(MEMORY_SIZE, 0);
}

uint16_t Memory::read(uint16_t address) {
    validateAddress(address);

    return memory[address];
}

void Memory::write(uint16_t address, uint16_t data) {
    validateAddress(address);

    memory[address] = (data >> 8) & 0x0F;
}