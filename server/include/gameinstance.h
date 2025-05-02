#pragma once

#include <vector>
#include <thread>
#include <mutex>

#include "player.h"
#include "rectangle.h"
#include "level.h"

#include "winsock2.h"

class GameInstance
{
private:
    // client thread will write to this (need mutex!), UpdateSimulation only reads from this (no mutex needed??)
    std::vector<Player> players;

    int maxPlayers;

    std::mutex playerVectorMutex;

    std::vector<GameRectangle> platforms = level::levelPlatforms;

public:
    GameInstance() {};
    GameInstance(int maxPlayers);
    ~GameInstance();

    // TODO - CALLED BY A NETWORKING FUNCTION, RETURNS A REF TO THE NEW PLAYER IN THE STRUCT
    int CreateANewPlayer();
    void RemovePlayer(int at);

    const Player& GetPlayerByNumber(int playerno);
    void UpdateSpecificPlayerInputs(int playerno, bool left, bool right, bool jump);

    std::vector<char> CreateSnapshot();

    void UpdateSimulation(); // for all players, make them move. Handle any collisions, report any losses
};

GameInstance::GameInstance(int maxPlayers)
{
    players.reserve(maxPlayers);
}

GameInstance::~GameInstance()
{
}

inline int GameInstance::CreateANewPlayer()
{
    std::lock_guard<std::mutex> lock(playerVectorMutex);
    players.push_back((Player){(int)players.size(), 100, 100, 0, 0, 32, 32, false, false, 0}); //TODO - CREATE A NEW PLAYER@@@@@@@@!!!!!!!!
    return players.size();
}

inline void GameInstance::RemovePlayer(int at)
{
    std::lock_guard<std::mutex> lock(playerVectorMutex);
    players.erase(players.begin() + at);
}

inline const Player &GameInstance::GetPlayerByNumber(int playerno)
{
    // if (playerno < 0 || playerno > players.size())
    //     return;
    // else
        return players.at(playerno);
}

inline void GameInstance::UpdateSpecificPlayerInputs(int playerno, bool left, bool right, bool jump)
{
    std::lock_guard<std::mutex> lock(playerVectorMutex);
    players.at(playerno).dx = (left ? -5 : 0);
    players.at(playerno).dx = (right ? 5 : 0);
    players.at(playerno).jumpRequest = jump;
}

inline int32_t ToNetInt(int val) {
    return static_cast<int32_t>(htonl(static_cast<uint32_t>(val)));
}

inline std::vector<char> GameInstance::CreateSnapshot()
{
    std::vector<char> buffer;

    // header
    buffer.push_back(protocol::SERVER_STATE_HEADER);

    // Player count
    uint32_t count = static_cast<uint32_t>(players.size());
    uint32_t netCount = htonl(count);
    buffer.insert(buffer.end(), reinterpret_cast<char*>(&netCount), reinterpret_cast<char*>(&netCount) + sizeof(uint32_t));

    // add each player, converting each field to network byte order
    for (const auto& p : players) {
        PlayerState state {
            ToNetInt(p.number),
            ToNetInt(p.x),
            ToNetInt(p.y),
            ToNetInt(p.width),
            ToNetInt(p.height),
            ToNetInt(p.score)
        };

        buffer.insert(buffer.end(), reinterpret_cast<char*>(&state), reinterpret_cast<char*>(&state) + sizeof(PlayerState));
    }

    return buffer;
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
        if (a.y < b.y) return 1;
        else return -1;
    }
    return 0;
}

inline void GameInstance::UpdateSimulation()
{
    for(Player& player : players)
    {
        // horizontal movement
        player.x += player.dx;
        moveToZero(player.x, 2);

        // jump
        if(player.jumpRequest && player.isOnGround)
            player.dy -= 20;
            player.isOnGround = false;
            player.jumpRequest = false;
        
        // gravity
        if (!player.isOnGround)
            player.dy += 5;

        player.y += player.dy;
        moveToZero(player.y, 2);

        // collision w/ ground/platforms
        player.isOnGround = false;
        GameRectangle playerRect = (GameRectangle){player.x, player.y, player.width, player.height};
        for(const auto& plat : platforms)
        {
            if (playerRect.intersects(plat)) {
                // snap to top of platform
                player.y = plat.y - player.height;
                player.dy = 0;
                player.isOnGround = true;
                break;
            }
        }
    }

    for (int i = 0; i < players.size(); i++)
    {
        for (int j = 0; j < players.size(); j++)
        {
            int col = CheckCollision(players.at(i), players.at(j));
            if (col >= 1)
            {
                // TODO - player 1 has scored! ADD MUTEX?
                players.at(i).score++;
            }
            else if (col <= -1)
            {
                // TODO - player 2 has scored!
                players.at(j).score++;
            }
        }
    }
}
