#pragma once

#include "clientstate.h"
#include "clientnetworking.h"
#include "raylib.h"

// Wait for user input
class ConnectingScreen
{
private:
    ClientNetworking& networkingRef;
public:
    ConnectingScreen(ClientNetworking& networkingRef);
    ~ConnectingScreen();

    void Update(ClientState& stateRef);
    void Draw(RenderTexture2D& renderTexture);
};

