#include "screen_title.h"
#include "screen_connecting.h"

#include <string>
#include <iostream>

#include "gui.h"

TitleScreen::TitleScreen(ClientNetworking& networkingRef) : networkingRef(networkingRef), startNetworking(false)
{
}

TitleScreen::~TitleScreen()
{
}

Screen* TitleScreen::Update(ClientState &stateRef)
{
    if(startNetworking) //Button clicked --> set state
    {
        stateRef = ClientState::STATE_CONNECTING;
        return new ConnectingScreen(this->networkingRef);
    }
    return nullptr;
}

void TitleScreen::Draw(RenderTexture2D& renderTexture, const Vector2& mousePos)
{
    BeginTextureMode(renderTexture);
        ClearBackground(RAYWHITE);

        DrawText("Server IP address", 200, 200, 30, BLACK);
        DrawText("Server Port Number", 750, 200, 30, BLACK);

        if (GUI::TextBox((Rectangle){250, 240, 200, 50}, ipTextBox, sizeof(ipTextBox), mousePos))
        {
        }

        if (GUI::TextBox((Rectangle){800, 240, 200, 50}, portTextBox, sizeof(portTextBox), mousePos))
        {
        }

        if(GUI::TextButton((Rectangle){450, 650, 170, 50}, "Connect", mousePos, BLUE, BLACK) && !startNetworking)
        {
            int portNum = atoi(portTextBox);
            std::string ipNum = std::string(ipTextBox);

            bool success = networkingRef.SetIPAndPort(ipNum, portNum);
            if(success)
                startNetworking = true;
            else
            {
                
            }
        }

    EndTextureMode();
}
