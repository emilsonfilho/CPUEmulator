#include "../../../include/System/CPU/CPU.hpp"

void CPU::loadProgram(const string& filename) {
    ifstream file("programs/" + filename + ".txt");
    if (!file.is_open()) {
        cerr << "Error: Nao foi possivel abrir o arquivo " << filename << endl;
        exit(1);
    }

    string line;
    while (getline(file, line)) {
        uint16_t memoryAddress = stoi(line.substr(0, 4), nullptr, 16); // Armazena em hexadecimal o endereco de memoria
    }
}