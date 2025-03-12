#include "player.h"
#include "winsock2.h"

std::vector<uint32_t> Player::ToNetworkBytes()
{
    // std::vector<uint8_t> rval = std::vector<uint8_t>();
    // rval.resize(2);
    // rval[0] = this->xPos;
    // rval[1] = this->yPos;
    // return rval;
    return { htonl(xPos), htonl(yPos) };
}

Player Player::FromNetworkBytes(std::vector<uint32_t> bytes)
{
    Player p;
    p.SetX(ntohl(bytes[0]));
    p.SetY(ntohl(bytes[1]));
}
