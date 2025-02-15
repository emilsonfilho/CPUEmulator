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