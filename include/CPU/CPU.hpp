#ifndef CPU_HPP
#define CPU_HPP

#include <string>
#include <cstdint>

using namespace std;

class CPU {
public:
    uint16_t R[8] = {0};
    uint16_t PC = 0;
    uint16_t SP = 0x8200;
    uint16_t IR = 0;

    CPU();
    
    void loadProgram(const string& program);
    void execute(uint16_t instruction);


};

#endif