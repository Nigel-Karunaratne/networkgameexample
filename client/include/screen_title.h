#pragma once

#include "clientstate.h"
#include "raylib.h"

class TitleScreen
{
private:
    char ipTextBox[16] = {0};
public:
    TitleScreen();
    ~TitleScreen();

    void Update(ClientState& stateRef);
    void Draw(RenderTexture2D& renderTexture);
};

