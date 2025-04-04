#pragma once

#include "clientstate.h"
#include "clientnetworking.h"
#include "raylib.h"

// Wait for user input
class TitleScreen
{
private:
    char ipTextBox[16] = {0};
    char portTextBox[6] = {0};
    bool startNetworking = false;
    ClientNetworking& networkingRef;
public:
    TitleScreen(ClientNetworking& networkingRef);
    ~TitleScreen();

    void Update(ClientState& stateRef);
    void Draw(RenderTexture2D& renderTexture, Vector2& mousePos);
};

