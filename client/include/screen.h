#pragma once

#include "clientstate.h"
#include "clientnetworking.h"
#include "raylib.h"

class Screen
{
private:
    Screen(Screen& screen);
public:
    Screen() {};
    virtual Screen* Update(ClientState& stateRef) = 0;
    virtual void Draw(RenderTexture2D& renderTexture, const Vector2& mousePos) = 0;
};
