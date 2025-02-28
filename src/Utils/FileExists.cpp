#include "../../include/Utils/FileExists.hpp"

bool fileExists(const string& filename) {
    ifstream file(Path::getFullPath(filename));
    return file.good();
}