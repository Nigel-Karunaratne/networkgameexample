#pragma once

#include "stdint.h"

struct Player
{
    int number; //NUMBER determines player color as well, on client side
    int x;
    int y;
    int dx;
    int dy;

    int width = 32;
    int height = 32;

    bool isOnGround = false;
    bool jumpRequest = false;
    int score = 0;
};

// no padding
#pragma pack(push,1)
struct PlayerState {
    int32_t id;
    int32_t x, y;
    int32_t width, height;
    int32_t score;
};
#pragma pack(pop)