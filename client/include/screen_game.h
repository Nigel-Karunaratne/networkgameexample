#pragma once

#include "screen.h"

class GameScreen : public Screen
{
private:
    ClientNetworking& networkingRef;
public:
    GameScreen(ClientNetworking& networkingRef);
    ~GameScreen();

    Screen* Update(ClientState& stateRef) override;
    void Draw(RenderTexture2D& renderTexture, const Vector2& mousePos) override;
};

