#include "../../../include/System/Memory/Memory.hpp"

/* Private methods */
void Memory::validateAddress(uint16_t address)
{
    if (address >= MEMORY_SIZE)
        throw OutOfMemoryException();
}

string Memory::printMemory(uint16_t startAddr, uint16_t endAddr, bool stack) const {
    ostringstream os;
    bool empty = true;

    for (size_t i = startAddr; (stack ? i > endAddr : i < endAddr); stack ? i-- : i++) {
        // Verifica se a memória foi acessada
        bool accessed = indexesAccessed.find(i) != indexesAccessed.end();

        // Se a memória não foi acessada, coloca "<memory not used>"
        if (accessed) {
            os << applyFormat(i) << ": "
               << applyFormat((int)memory[i], 2) << endl;
        }

        empty = false;
    }

    if (empty) {
        return stack ? "<empty>\n" : "<memory not used>\n";
    }

    return os.str();
}

/* Public methods */
Memory::Memory()
{
    memory.resize(MEMORY_SIZE, 0);
}

uint16_t Memory::read(uint16_t address)
{
    validateAddress(address);

    return (memory[address] << 8) | memory[address + 1];
}

void Memory::write(uint16_t address, uint16_t data)
{
    validateAddress(address);

    memory[address] = (data >> 8) & 0xFF;
    memory[address + 1] = data & 0xFF;
}

string Memory::toString() const {
    return printMemory(0, 0xFFFF);
}

string Memory::getStack(uint16_t SP) const {
    return printMemory(SP, 0x8200, true);
}