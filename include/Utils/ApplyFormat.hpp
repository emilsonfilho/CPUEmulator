#ifndef APPLY_FORMAT_HPP
#define APPLY_FORMAT_HPP

#include <sstream>
#include <iomanip>

using namespace std;

string applyFormat(uint16_t value, int width = 4, char fill = '0');

#endif