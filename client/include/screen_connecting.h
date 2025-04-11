#pragma once

#include "screen.h"

// Wait for user input
class ConnectingScreen : public Screen
{
private:
    ClientNetworking& networkingRef;
    float delayTimer;
public:
    ConnectingScreen(ClientNetworking& networkingRef);
    ~ConnectingScreen();

    Screen* Update(ClientState& stateRef) override;
    void Draw(RenderTexture2D& renderTexture, const Vector2& mousePos) override;
};

