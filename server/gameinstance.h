#pragma once

#include <vector>

struct Player
{
    // int number; //NUMBER determines player color as well, on client side
    int x;
    int y;
    int dx;
    int dy;

    int width = 32;
    int height = 32;
};

class GameInstance
{
private:
    // client thread will write to this (need mutex!), UpdateSimulation only reads from this (no mutex needed??)
    std::vector<Player> players;

    int maxPlayers;
public:
    GameInstance() {};
    GameInstance(int maxPlayers);
    ~GameInstance();

    const Player& GetPlayerByNumber(int playerno);
    void UpdateSpecificPlayer(int playerno, Player newValues);

    void UpdateSimulation(); // for all players, make them move. Handle any collisions, report any losses
};

GameInstance::GameInstance(int maxPlayers)
{
    players.reserve(maxPlayers);
}

GameInstance::~GameInstance()
{
}

inline const Player& GameInstance::GetPlayerByNumber(int playerno)
{
    // if (playerno < 0 || playerno > players.size())
    //     return;
    // else
        return players.at(playerno);
}

inline void GameInstance::UpdateSpecificPlayer(int playerno, Player newValues)
{
    players.at(0) = newValues;
}

void moveToZero(int& value, int amount)
{
    if(amount < 0) return;
    if(value > 0 && value - amount > 0) value = value - amount;
    else if (value < 0 && value + amount < 0) value = value + amount;
    else value = 0;
}


// Returns 1 if A is higher than B, -1 if B is higher than A, or 0 if no collision.
int CheckCollision(const Player& a, const Player& b)
{
    if (a.x < b.x + b.width &&
        a.x + a.width > b.x &&
        a.y < b.y + b.height &&
        a.y + a.height > b.y)
    {
        if (a.y > b.y) return 1;
        else return -1;
    }
    return 0;
}

inline void GameInstance::UpdateSimulation()
{
    for(Player& player : players)
    {
        player.x += player.dx;
        moveToZero(player.x, 2);
        player.y += player.dy;
        moveToZero(player.y, 2);
    }

    for (int i = 0; i < players.size(); i++)
    {
        for (int j = 0; j < players.size(); j++)
        {
            int col = CheckCollision(players.at(i), players.at(j));
            if (col >= 1)
            {
                // TODO - player 1 has scored!
            }
            else if (col <= -1)
            {
                // TODO - player 2 has scored!
            }
        }
    }
}
