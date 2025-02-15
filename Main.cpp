#include "include/System/CPU/CPU.hpp"

int main() {
    Memory memory;
    CPU cpu(&memory);

    cpu.loadProgram("program1");
    cpu.runProgram();

    return 0;
}