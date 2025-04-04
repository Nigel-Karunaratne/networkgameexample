#include "screen_title.h"

#include <string>
#include <iostream>

#include "gui.h"

TitleScreen::TitleScreen(ClientNetworking& networkingRef) : networkingRef(networkingRef), startNetworking(false)
{
}

TitleScreen::~TitleScreen()
{
}

void TitleScreen::Update(ClientState &stateRef)
{
    if(startNetworking) //Button clicked --> set state
    {
        stateRef = ClientState::STATE_CONNECTING;
    }
}

void TitleScreen::Draw(RenderTexture2D& renderTexture, Vector2& mousePos)
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
