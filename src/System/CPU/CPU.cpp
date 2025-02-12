#include "../../../include/System/CPU/CPU.hpp"

/* Private methods */
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

        memory->write(memoryAddress, stoi(line.substr(8, 4), nullptr, 16)); // Armazena já na memória o dado que eu quero e o próximo
    }
}

void CPU::execute(uint16_t instruction) {
    uint8_t opcode = (instruction >> 12) & 0xF;
    switch (opcode)
    {
    case 0x1:
        MOV(instruction & 0x0FFF); // Talvez eu fça uma função para simplificar o processo de não mandar mais o bit que não precisa
        break;
    
    default:
        break;
    }
}

void CPU::runProgram() {
    execute(memory->read(PC));

    PC += 2;
}