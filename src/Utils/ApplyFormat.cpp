#include "../../include/Utils/ApplyFormat.hpp"

string applyFormat(uint16_t value, bool preffixed, int width, char fill) {
    ostringstream os;
    os << (preffixed ? "0x" : "") << hex << setw(width) << setfill(fill) << value;
    return os.str();
}