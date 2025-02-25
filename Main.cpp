#include "include/System/CPU/CPU.hpp"

/**
 * Para usar o programa, basta compilar o código com o comando: make
 * Ou executar o arquivo ./app
 * 
 * Integrantes:
 * - Thalisson Gesian Perdigão Rocha - Matrícula 565925
 * - Grazielly Lima Souza - Matrícula 567449
 * - Samira Gomes Donato - Matrícula 568383
*/
int main() {
    Memory memory;
    CPU cpu(&memory);

    cpu.loadProgram("program1");
    cpu.runProgram();

    return 0;
}