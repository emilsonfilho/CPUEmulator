#ifndef CPU_MASKS_HPP
#define CPU_MASKS_HPP

#include <cstdint>

const uint16_t MASK_TYPE_BIT = 0x0800;
const uint16_t MASK_DATA = 0x07FF;
const uint16_t MASK_DEST = 0x0700;
const uint16_t MASK_ORIGIN = 0x00E0;
const uint16_t MASK_IMMEDIATE = 0x00FF;
const uint16_t MASK_DEST_STR = 0x0700;
const uint16_t MASK_ORIGIN_STR = 0x00E0;
const uint16_t MASK_IMMEDIATE_STR = 0x071F;
const uint16_t MASK_LSB_STR = 0x01F;

#endif