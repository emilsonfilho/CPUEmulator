#include "../../include/Utils/GetProgramName.hpp"

string getProgramName() {
    while (true) {
        try {
            cout << "Digite o nome do arquivo de programa: ";
            string programName;
            cin >> programName;

            if (!fileExists(programName)) throw FileNotFoundException();

            return programName;
        } catch (const FileNotFoundException& e) {
            cout << e.what() << endl;
        }
    }
}