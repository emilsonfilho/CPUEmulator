#include "../../../include/System/CPU/Flags.hpp"

string Flags::showFlags() const {
    ostringstream os;

    os << (C ? "C " : "- ");
    os << (Ov ? "Ov " : "- ");
    os << (Z ? "Z " : "- ");
    os << (S ? "S " : "- ");

    os << endl;

    return os.str();
}
void Flags::setZero(uint32_t result)  {
    Z = (result & 0xFFFF) == 0;
}

void Flags::setSign(uint32_t result) {
    S = (result & 0x8000) != 0;
}