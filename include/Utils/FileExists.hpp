#ifndef FILE_EXISTS_HPP
#define FILE_EXISTS_HPP

#include <string>
#include <fstream>

#include "../../include/Global/FilesPath.hpp"

using namespace std;

bool fileExists(const string& filename);

#endif