#include "screen_connecting.h"
#include "screen_game.h"

#include <iostream>

ConnectingScreen::ConnectingScreen(ClientNetworking &networkingRef) : networkingRef(networkingRef), delayTimer(0.0f)
{
    networkingRef.SetupNetworkingThread();
}

ConnectingScreen::~ConnectingScreen()
{
}

Screen* ConnectingScreen::Update(ClientState &stateRef)
{
    delayTimer += GetFrameTime();
    if (delayTimer >= 1.0f && networkingRef.HasBeenAcceptedByServer())
    {
        // TODO - create some structure for current game State??
        return new GameScreen(this->networkingRef);
    }
    return nullptr;
}

void ConnectingScreen::Draw(RenderTexture2D &renderTexture, const Vector2& mousePos)
{
    BeginTextureMode(renderTexture);
        ClearBackground(BLACK);
        std::string text = "Connecting to " + networkingRef.GetAddressRepresentation();  
        DrawText(text.c_str(), 600, 400, 48, WHITE);
    EndTextureMode();
}
