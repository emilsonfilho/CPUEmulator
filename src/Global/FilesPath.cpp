#include "../../include/Global/FilesPath.hpp"

const string Path::DIR = "programs/";
const string Path::EXTENSION = ".txt";
const string Path::getFullPath(const string& filename) {
    return DIR + filename + EXTENSION;
}