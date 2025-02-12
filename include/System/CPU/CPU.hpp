#ifndef CPU_HPP
#define CPU_HPP

#include <string>
#include <cstdint>
#include <fstream>
#include <iostream>

#include "../Memory/Memory.hpp"

using namespace std;

class CPU {
    uint16_t R[8] = {0};
    uint16_t PC = 0x0000;
    uint16_t SP = 0x8200;
    uint16_t IR = 0;

    Memory *memory;

    void NOP();
    void HALT();
    void MOV(uint16_t data);
    void LRD();
    void ADD();
    void SUB();
    void MUL();
    void AND();
    void ORR();
    void NOT();
    void XOR();
    void PSH();
    void POP();
    void CMP();
    void JMP();
    void JEQ();
    void JLT();
    void JGT();
    void SHR();
    void SHL();
    void ROR();
    void ROL();

    void execute(uint16_t instruction);
public:
    CPU(Memory* memory);
    
    void loadProgram(const string& filename);
    void runProgram();
};

#endif