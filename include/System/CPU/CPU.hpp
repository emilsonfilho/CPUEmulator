#ifndef CPU_HPP
#define CPU_HPP

#include <fstream>
#include <iostream>
#include <filesystem>

#include "../Memory/Memory.hpp"
#include "../../Masks/CPUMasks.hpp"
#include "../../Utils/ClearTop4Bits.hpp"
#include "Flags.hpp"
#include "AddressOperands.hpp"
#include "Operands.hpp"

#include "../../Exceptions/StackOverflowException.hpp"
#include "../../Exceptions/StackUnderflowException.hpp"

using namespace std;

class CPU {
    uint16_t R[8] = {0};
    uint16_t PC = 0x0000;
    uint16_t SP = 0x8200;
    uint16_t IR = 0;

    Memory *memory;
    Flags flags;

    string generateLog() const;
    void saveLogFile(const string& data) const;
    string showRegisters() const;

    UnsignedOperands getUnsignedOperands(uint16_t numberRegister1, uint16_t numberRegister2);
    SignedOperands getSignedOperands(uint16_t numberRegister1, uint16_t numberRegister2);

    UnsignedOperands getUnsignedOperandsFromData(uint16_t data);
    SignedOperands getSignedOperandsFromData(uint16_t data);

    uint16_t getULADestination(uint16_t data);

    void setResultInRegister(uint32_t result, uint16_t address);

    uint8_t getShiftImmediate(uint16_t data);

    void setFlags(uint32_t data);
    void setOverflowAddSub(uint16_t op1, uint16_t op2, uint32_t result, bool isSubstraction);
    void setCarryAddSub(uint16_t op1, uint16_t op2, uint32_t result, bool isSubstraction);
    void setCarryMul(int32_t result);
    void setShiftCarry(uint16_t op1, bool isMSB = true);
    void cleanOverflow();
    void cleanCarryAndOverflow();

    void NOP();
    void HALT();
    void MOV(uint16_t data);
    void STR(uint16_t data);
    void LDR(uint16_t data);
    void ADD(uint16_t data);
    void SUB(uint16_t data);
    void MUL(uint16_t data);
    void AND(uint16_t data);
    void ORR(uint16_t data);
    void NOT(uint16_t data);
    void XOR(uint16_t data);
    void PSH(uint16_t data);
    void POP(uint16_t data);
    void CMP(uint16_t data);
    void JMP();
    void JEQ();
    void JLT();
    void JGT();
    void SHR(uint16_t data);
    void SHL(uint16_t data);
    void ROR(uint16_t data);
    void ROL(uint16_t data);

    void execute(uint16_t instruction);
public:
    static AddressOperands decodeAddressOperands(uint16_t data);
    static uint16_t applyMask(uint16_t data);
    static uint8_t getMSB(uint16_t data);
    static uint8_t getLSB(uint16_t data);

    CPU(Memory* memory);
    
    void loadProgram(const string& filename);
    void runProgram();
};

#endif