#include "screen_connecting.h"
#include "screen_game.h"
#include "screen_title.h"

#include "gui.h"

#include <iostream>

ConnectingScreen::ConnectingScreen(ClientNetworking &networkingRef) : networkingRef(networkingRef), delayTimer(0.0f), acceptedByServerStatus(0), returnToTitle(false)
{
    this->successfullySetupNetworking = networkingRef.SetupNetworkingThread();
    if (!successfullySetupNetworking)
        networkingRef.ShutdownNetworkngThread();
}

ConnectingScreen::~ConnectingScreen()
{
}

Screen* ConnectingScreen::Update(ClientState &stateRef)
{
    if(returnToTitle)
    {
        networkingRef.ShutdownNetworkngThread();
        return new TitleScreen(this->networkingRef);
    }
    if(delayTimer < 1.0f)
    {
        delayTimer += GetFrameTime();
    }
    else
    {
        acceptedByServerStatus = networkingRef.GetConnectionRequestStatus();
        if(acceptedByServerStatus > 1)
        {
            // TODO - create some structure for current game State??
            // return new GameScreen(this->networkingRef);
        }
    }

    return nullptr;
}

void ConnectingScreen::Draw(RenderTexture2D &renderTexture, const Vector2& mousePos)
{
    BeginTextureMode(renderTexture);
        ClearBackground(BLACK);
        std::string text = "Connecting to " + networkingRef.GetAddressRepresentation();  
        DrawText(text.c_str(), 300, 200, 48, WHITE);

        if (!successfullySetupNetworking)
        {
            DrawText("An error occured when setting up networking.", 300, 520, 36, RED);
        }
        if(acceptedByServerStatus == -1)
        {
            DrawText("Server Rejected Request", 300, 500, 36, RED);
        }

        if(GUI::TextButton((Rectangle){450, 650, 170, 50}, "Return", mousePos, RED, WHITE))
        {
            returnToTitle = true;
        }

    EndTextureMode();
}
