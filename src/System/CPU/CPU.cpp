#include "../../../include/System/CPU/CPU.hpp"

/* Private methods */
string CPU::generateLog() const {
    ostringstream os;

    os << "REGISTRADORES:\n"
       << showRegisters();

    os << endl;

    os << "MEMORIA DE DADOS:\n"
       << memory->toString();

    os << endl;

    os << "PILHA:\n"
       << memory->getStack(SP);

    os << endl;

    os << "FLAGS:\n"
       << flags.showFlags();

    return os.str();
}

void CPU::saveLogFile(const string& data) const {
    ofstream logfile("logs/log.txt");

    if (!logfile) {
        cerr << "Erro ao abrir o arquivo de log!\n";
        return;
    }

    logfile << data;
}

string CPU::showRegisters() const {
    ostringstream os;

    for (int i = 0; i < 8; i++) {
        os << "R" << i << ": " << applyFormat(R[i]) << endl;
    }

    os << "PC: " << applyFormat(PC) << endl
       << "SP: " << applyFormat(SP) << endl
       << "IR: " << applyFormat(IR) << endl;

    return os.str();
}

void CPU::setFlags(uint32_t data) {
    flags.setZero(data);
    flags.setSign(data);
}

void CPU::setOverflowAddSub(uint16_t op1, uint16_t op2, uint32_t result, bool isSubstraction) {
    bool signOp1 = (op1 & BIT_ISOLATION) != 0;
    bool signOp2 = (op2 & BIT_ISOLATION) != 0;
    bool signResult = (result & BIT_ISOLATION) != 0;

    flags.Ov = (isSubstraction ? (signOp1 != signOp2) : (signOp1 = signOp2)) and (signOp1 != signResult);
}

void CPU::setCarryAddSub(uint16_t op1, uint16_t op2, uint32_t result, bool isSubstraction) {
    flags.C = (isSubstraction ? op1 >= op2 : result > UPPER_LIMIT_REPRESENTATION);
}

void CPU::setCarryMul(int32_t result) {
    flags.C = result > UPPER_LIMIT_REPRESENTATION;
}

void CPU::setShiftCarry(uint16_t op1, bool isMSB) {
    flags.C = (isMSB ? (getMSB(op1)) : (getLSB(op1))) != 0;
}

void CPU::cleanOverflow() {
    flags.Ov = false;
}

void CPU::cleanCarryAndOverflow() {
    flags.C = false;
    cleanOverflow();
}

UnsignedOperands CPU::getUnsignedOperands(uint16_t numberRegister1, uint16_t numberRegister2) {
    UnsignedOperands operands;

    operands.op1 = R[numberRegister1];
    operands.op2 = R[numberRegister2];

    return operands;
}

SignedOperands CPU::getSignedOperands(uint16_t numberRegister1, uint16_t numberRegister2) {
    SignedOperands operands;

    operands.op1 = static_cast<int16_t>(R[numberRegister1]);
    operands.op2 = static_cast<int16_t>(R[numberRegister2]);

    return operands;
}

UnsignedOperands CPU::getUnsignedOperandsFromData(uint16_t data) {
    auto [origin, origin2] = decodeAddressOperands(data);
    return getUnsignedOperands(origin, origin2);
}

SignedOperands CPU::getSignedOperandsFromData(uint16_t data) {
    auto [origin, origin2] = decodeAddressOperands(data);
    return getSignedOperands(origin, origin2);
}

void CPU::setResultInRegister(uint32_t result, uint16_t address) {
    R[address] = (uint16_t)(result & UPPER_LIMIT_REPRESENTATION);
}

uint16_t CPU::getULADestination(uint16_t data) {
    return (data & MASK_DEST) >> 8;
}

uint8_t CPU::getShiftImmediate(uint16_t data) {
    return (data & MASK_IMMEDIATE_SHIFT);
}

void CPU::NOP() {
    const string data = generateLog();
    saveLogFile(data);

    cout << data;
}

void CPU::MOV(uint16_t data) {
    bool isForRegistrar = (data & MASK_TYPE_BIT) == 0;
    data = applyMask(data); // Máscara para tirar o bit de tipo da jogada
    
    int dest = (data & MASK_DEST) >> 8;

    if (isForRegistrar) {
        // Sequência na forma 0000 0RRR rrr- ----
        int origin = (data & MASK_ORIGIN) >> 5;

        R[dest] = R[origin];
    } else {
        // Sequência na forma 0000 0RRR IIII IIII
        R[dest] = data & MASK_IMMEDIATE;
    }
}

void CPU::HALT() {
    exit(0);
}

void CPU::STR(uint16_t data) {
    bool isForRegister = (data & MASK_TYPE_BIT) == 0;
    data = applyMask(data);

    uint16_t dest = (data & MASK_DEST_STR) >> 5;

    if (isForRegister) {
        // Sequência na forma 0000 0--- RRRr rr--
        int origin = (data & MASK_ORIGIN_STR) >> 2;

        memory->write(R[dest], R[origin]);
    } else {
        // Sequência na forma 0000 0III RRRI IIII
        memory->write(R[dest], (data & MASK_LSB_STR) | ((data & MASK_MSB_STR) >> 3));
        memory->indexesAccessed.insert(R[dest]);
    }
}

void CPU::LDR(uint16_t data) {
    /**
     * Data na forma 0011 -RRR rrr- ----
     * Máscara de isolamento é a mesma que a de MOV
    */

    int dest = (data & MASK_DEST) >> 8;
    int origin = (data & MASK_ORIGIN) >> 5;

    R[dest] = memory->read(R[origin]);
}

void CPU::ADD(uint16_t data) {
    data = applyMask(data);
    
    uint16_t dest = getULADestination(data);
    auto [op1, op2] = getUnsignedOperandsFromData(data);

    uint32_t result = (uint32_t)op1 + (uint32_t)op2;

    setResultInRegister(result, dest);

    setCarryAddSub(op1, op2, result, false);
    setFlags(result);
    setOverflowAddSub(op1, op2, result, false);
}

void CPU::SUB(uint16_t data) {
    data = applyMask(data);
    
    uint16_t dest = getULADestination(data);
    auto [op1, op2] = getUnsignedOperandsFromData(data);

    uint32_t result = (uint32_t)op1 - (uint32_t)op2;

    setResultInRegister(result, dest);

    setCarryAddSub(op1, op2, result, true);
    setFlags(result);
    setOverflowAddSub(op1, op2, result, true);
}

void CPU::MUL(uint16_t data) {
    data = applyMask(data);

    uint16_t dest = getULADestination(data);
    auto [op1, op2] = getSignedOperandsFromData(data);

    int32_t result = (int32_t)op1 * (int32_t)op2;

    setResultInRegister(result, dest);

    int16_t minValue = 0x8000, maxValue = 0x7FFF;
    flags.Ov = (result < minValue) or (result > maxValue);

    setCarryMul(result);
    setFlags(result);
}

void CPU::AND(uint16_t data) {
    data = applyMask(data);

    uint16_t dest = getULADestination(data);
    auto [op1, op2] = getUnsignedOperandsFromData(data);

    uint16_t result = op1 & op2;

    setResultInRegister(result, dest);    

    cleanCarryAndOverflow();
    setFlags(result);
}

void CPU::ORR(uint16_t data) {
    data = applyMask(data);

    uint16_t dest = getULADestination(data);
    auto [op1, op2] = getUnsignedOperandsFromData(data);

    uint16_t result = op1 | op2;

    setResultInRegister(result, dest);

    cleanCarryAndOverflow();
    setFlags(result);
}

void CPU::NOT(uint16_t data) {
    data = applyMask(data);

    uint16_t dest = getULADestination(data);
    auto [op1, _] = getUnsignedOperandsFromData(data);

    uint16_t result = ~op1;

    setResultInRegister(result, dest);

    cleanCarryAndOverflow();
    setFlags(result);
}

void CPU::XOR(uint16_t data) {
    data = applyMask(data);

    uint16_t dest = getULADestination(data);
    auto [op1, op2] = getUnsignedOperandsFromData(data);

    uint16_t result = op1 ^ op2;

    setResultInRegister(result, dest);

    cleanCarryAndOverflow();
    setFlags(result);
}

void CPU::PSH(uint16_t data) {
    if (SP <= 0x81F0)
        throw StackOverflowException();

    data = applyMask(data);

    auto [_, op] = getUnsignedOperandsFromData(data);

    memory->write(SP, op);
    SP -= 2;
}

void CPU::POP(uint16_t data) {
    if (SP >= 0x8200)
        throw StackUnderflowException();

    data = applyMask(data);

    SP += 2;

    uint16_t dest = getULADestination(data);
    setResultInRegister(memory->read(SP), dest);
}

void CPU::CMP(uint16_t data) {
    data = applyMask(data);

    auto [op1, op2] = getUnsignedOperandsFromData(data);

    flags.Z = (op1 == op2) ? true : false;
    flags.C = (op1 < op2) ? true : false;
}

void CPU::JMP(uint16_t data) {
    data = applyMask(data);

    uint16_t immediate = (data & IMMEDIATE_MASK_JUMPS) >> 2;

    PC += immediate;
}

void CPU::JEQ(uint16_t data) {

}

void CPU::SHR(uint16_t data) {
    data = applyMask(data);

    uint16_t dest = getULADestination(data);
    auto [op1, _] = getUnsignedOperandsFromData(data);

    setShiftCarry(op1, false);

    uint16_t result = op1 >> getShiftImmediate(data);

    setResultInRegister(result, dest);

    cleanOverflow();
    setFlags(result);
}

void CPU::SHL(uint16_t data) {
    data = applyMask(data);

    uint16_t dest = getULADestination(data);
    auto [op1, _] = getUnsignedOperandsFromData(data);

    setShiftCarry(op1);

    uint16_t result = op1 << getShiftImmediate(data);

    setResultInRegister(result, dest);

    cleanOverflow();
    setFlags(result);
}

void CPU::ROR(uint16_t data) {
    data = applyMask(data);

    uint16_t dest = getULADestination(data);
    auto [op1, _] = getUnsignedOperandsFromData(data);

    uint16_t op1LSB = getLSB(op1);

    setShiftCarry(op1, false);

    uint16_t result = op1 >> 1;
    
    if (op1LSB == 1)
        result = result | 0x8000;

    setResultInRegister(result, dest);

    cleanOverflow();
    setFlags(result);
}

void CPU::ROL(uint16_t data) {
    data = applyMask(data);

    uint16_t dest = getULADestination(data);
    auto [op1, _] = getUnsignedOperandsFromData(data);

    uint16_t op1MSB = getMSB(op1);

    setShiftCarry(op1);

    uint16_t result = op1 << 1;

    if (op1MSB == 1)
        result = result | 0x0001;
    
    setResultInRegister(result, dest);
    
    cleanOverflow();
    setFlags(result);
}

/* Public methods */
AddressOperands CPU::decodeAddressOperands(uint16_t data) {
    AddressOperands operands;

    operands.origin = (data & MASK_ORIGIN) >> 5;
    operands.origin2 = (data & MASK_ORIGIN_STR) >> 2;

    return operands;
}

uint16_t CPU::applyMask(uint16_t data) {
    return (data & MASK_DATA);
}

uint8_t CPU::getMSB(uint16_t data) {
    return ((data & MSB_ISOLATION_MASK) >> 15);
}

uint8_t CPU::getLSB(uint16_t data) {
    return (data & LSB_ISOLATION_MASK);
}

CPU::CPU(Memory* memory): memory(memory) {}

void CPU::loadProgram(const string& filename) {
    ifstream file("programs/" + filename + ".txt");
    if (!file.is_open()) {
        cerr << "Error: Nao foi possivel abrir o arquivo " << filename << endl;
        exit(1);
    }

    /* Assmimos que o programa sempe começa no 0x0000 */

    string line;
    while (getline(file, line)) {
        uint16_t memoryAddress = stoi(line.substr(0, 4), nullptr, 16); // Armazena em hexadecimal o endereco de memoriab
        memory->indexesAccessed.insert(memoryAddress);
        memory->indexesAccessed.insert(memoryAddress + 1);

        memory->write(memoryAddress, stoi(line.substr(8, 4), nullptr, 16)); // Armazena já na memória o dado que eu quero e o próximo
    }
}


void CPU::execute(uint16_t instruction) {
    uint8_t opcode = (instruction >> 12) & 0xF;

    switch (opcode)
    {
    case 0x0: {
        if (instruction == 0x00000) {
            NOP();
            break;
        }

        uint8_t typeBit = ((instruction & MASK_TYPE_BIT) >> 10);
        if (typeBit == 0) {
            /**
             * Três opções;
             * PSH, POP e CMP
             */
            uint8_t diffBit = (instruction & DIFFERENTIATION_PAIR_BIT);
            switch (diffBit)
            {
            case 0x1:
                PSH(instruction);
                break;
            case 0x2:
                POP(instruction);
                break;
            case 0x3:
                CMP(instruction);
                break;
            default:
                break;
            }
        } else {
            /**
             * Quatro opções:
             * JMP, JEQ, JLT, JGT
             */
        }
        break;
    }
    case 0x1:
        MOV(instruction); // Talvez eu fça uma função para simplificar o processo de não mandar mais o bit que não precisa
        break;
    case 0x2:
        STR(instruction);
        break;
    case 0x3:
        LDR(instruction);
        break;
    case 0x4:
        ADD(instruction);
        break;
    case 0x5:
        SUB(instruction);
        break;
    case 0x6:
        MUL(instruction);
        break;
    case 0x7:
        AND(instruction);
        break;
    case 0x8:
        ORR(instruction);
        break;
    case 0x9:
        NOT(instruction);
        break;
    case 0xA:
        XOR(instruction);
        break;
    case 0xB:
        SHR(instruction);
        break;
    case 0xC:
        SHL(instruction);
        break;
    case 0xD:
        ROR(instruction);
        break;
    case 0xE:
        ROL(instruction);
        break;
    case 0xFF:
        HALT();
        break;
    default:

        break;
    }
}

void CPU::runProgram() {
    while (PC < Memory::MEMORY_SIZE) {  // Verifica se o PC não ultrapassa o tamanho da memória
        uint16_t instruction = memory->read(PC);
        
        if (instruction == 0xFFFF) {  // HALT pode ser identificado por um valor especial
            HALT();
            break;
        }

        uint16_t oldPC = PC;

        execute(instruction);

        if (oldPC == PC)
            PC += 2;  // Avança para a próxima instrução (assumindo instruções de 2 bytes)
    }
}
