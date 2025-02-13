#include "../../include/Utils/ClearTop4Bits.hpp"

uint16_t clearTop4Bits(uint16_t data) {
    return data & 0x0FFF;
}