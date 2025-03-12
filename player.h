#pragma once
#include <stdint.h>
#include <vector>

class Player
{
private:
    int xPos;
    int yPos;
public:
    Player() : xPos(0), yPos(0) {};
    ~Player() {};

    int GetX() { return xPos; };
    int GetY() { return yPos; };

    void SetX(int x) {xPos = x;};
    void SetY(int y) {yPos = y;};

    std::vector<uint32_t> ToNetworkBytes();
    Player FromNetworkBytes(std::vector<uint32_t> bytes);
};
