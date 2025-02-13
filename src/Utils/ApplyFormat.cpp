#include "../../include/Utils/ApplyFormat.hpp"

string applyFormat(uint16_t value, int width, char fill) {
    ostringstream os;
    os << hex << setw(width) << setfill(fill) << value;
    return os.str();
}