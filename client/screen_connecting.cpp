#include "screen_connecting.h"
#include <iostream>

ConnectingScreen::ConnectingScreen(ClientNetworking &networkingRef) : networkingRef(networkingRef)
{
}

ConnectingScreen::~ConnectingScreen()
{
}

void ConnectingScreen::Update(ClientState &stateRef)
{
}

void ConnectingScreen::Draw(RenderTexture2D &renderTexture)
{
    BeginTextureMode(renderTexture);
        ClearBackground(BLACK);
        std::string text = "Connecting to " + networkingRef.GetAddressRepresentation();  
        DrawText(text.c_str(), 600, 400, 48, WHITE);
    EndTextureMode();
}
