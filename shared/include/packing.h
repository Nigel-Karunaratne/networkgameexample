#pragma once

#include "stdint.h"

namespace packing
{
    const uint8_t INPUT_LEFT  = 1 << 0; // 0b00000001
    const uint8_t INPUT_RIGHT = 1 << 1; // 0b00000010
    const uint8_t INPUT_JUMP  = 1 << 2; // 0b00000100

    uint8_t PackInputToByte(bool left, bool right, bool jump) {
        uint8_t rval = 0;
        if (left)  rval |= INPUT_LEFT;
        if (right) rval |= INPUT_RIGHT;
        if (jump)  rval |= INPUT_JUMP;
        return rval;
    }

    void UnpackInputFromByte(uint8_t packedInput, bool& left, bool& right, bool& jump) {
        left  = packedInput & INPUT_LEFT;
        right = packedInput & INPUT_RIGHT;
        jump  = packedInput & INPUT_JUMP;
    }
    
} // namespace packing
