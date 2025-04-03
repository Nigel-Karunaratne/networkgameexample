#include "screen_title.h"
#include "raygui.h"

#include <string>
#include <iostream>

#include "gui.h"

TitleScreen::TitleScreen()
{
}

TitleScreen::~TitleScreen()
{
}

void TitleScreen::Update(ClientState &stateRef)
{
}

void TitleScreen::Draw(RenderTexture2D& renderTexture, Vector2& mousePos)
{
    BeginTextureMode(renderTexture);

        if (GUI::TextBox((Rectangle){10, 10, 200, 50}, ipTextBox, mousePos))
        {
            std::cout << ipTextBox << std::endl;
        }

        if (GUI::TextBox((Rectangle){10, 80, 200, 100}, ipTextBox, mousePos))
        {
            std::cout << ipTextBox << std::endl;
        }

        ClearBackground(RAYWHITE);
        DrawText("Congrats! You created your first window!", 190, 200, 20, LIGHTGRAY);
    EndTextureMode();
}
