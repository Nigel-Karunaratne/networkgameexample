#include "screen_title.h"
#include "raygui.h"

#include <string>
#include <iostream>

TitleScreen::TitleScreen()
{
}

TitleScreen::~TitleScreen()
{
}

void TitleScreen::Update(ClientState &stateRef)
{
}

void TitleScreen::Draw(RenderTexture2D& renderTexture)
{
    BeginTextureMode(renderTexture);

        if (GuiTextBox((Rectangle){10, 10, 200, 50}, ipTextBox, sizeof(ipTextBox), true))
        {
            std::cout << ipTextBox << std::endl;
        }

        ClearBackground(RAYWHITE);
        DrawText("Congrats! You created your first window!", 190, 200, 20, LIGHTGRAY);
    EndTextureMode();
}
