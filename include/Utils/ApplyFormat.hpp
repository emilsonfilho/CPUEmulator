#ifndef APPLY_FORMAT_HPP
#define APPLY_FORMAT_HPP

#include <sstream>
#include <iomanip>
#include <cstdint>

using namespace std;

string applyFormat(uint16_t value, bool preffixed = true, int width = 4, char fill = '0');

#endif