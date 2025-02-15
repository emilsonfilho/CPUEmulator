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

UnsignedOperands CPU::getUnsignedOperands(uint16_t numberRegister1, uint16_t numberRegister2) {
    UnsignedOperands operands;

    operands.op1 = R[numberRegister1];
    operands.op2 = R[numberRegister2];

    return operands;
}

SignedOperands CPU::getSignedOperands(uint16_t numberRegister1, uint16_t numberRegister2) {
    SignedOperands operands;

    operands.op1 = (int16_t)R[numberRegister1];
    operands.op2 = (int16_t)R[numberRegister2];

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
    auto [op1, op2] = getUnsignedOperandsFromData(data);

    int32_t result = (int32_t)op1 * (int32_t)op2;

    setResultInRegister(result, dest);

    int16_t minValue = 0x8000, maxValue = 0x7FFF;
    flags.Ov = (result < minValue) or (result > maxValue);

    setCarryMul(result);
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
    case 0x0:
        NOP();
        break;
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

        execute(instruction);
        PC += 2;  // Avança para a próxima instrução (assumindo instruções de 2 bytes)
    }
}
