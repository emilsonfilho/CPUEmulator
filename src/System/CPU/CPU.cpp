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

void CPU::NOP() {
    const string data = generateLog();
    saveLogFile(data);

    cout << data;
}

void CPU::MOV(uint16_t data) {
    bool isForRegistrar = (data & MASK_TYPE_BIT) == 0;
    data &= MASK_DATA; // Máscara para tirar o bit de tipo da jogada
    
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
    data &= MASK_DATA;

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

/* Public methods */
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
