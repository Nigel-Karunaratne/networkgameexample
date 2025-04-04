#include "gui.h"
#include "string.h"

#include <iostream>

bool GUI::TextBox(Rectangle bounds, char *textBuffer, int bufferSize, Vector2 mousePos)
{
    DrawRectangleLinesEx(bounds, 1.0f, BLACK);
    Vector2 textSize = MeasureTextEx(GetFontDefault(), textBuffer, 32, 1.0f);

    if(CheckCollisionPointRec(mousePos, bounds))
    {
        DrawText(textBuffer, bounds.x + 5, bounds.y + (bounds.height / 2) - (textSize.y / 2), 32, BLACK);

        int codepoint = GetCharPressed();
        int backSpacePressed = IsKeyPressed(KEY_BACKSPACE);
        int deletePressed = IsKeyPressed(KEY_DELETE);
        if (IsKeyPressed(KEY_ENTER)) return true; // codepoint = (int)'\n';
        int codepointSize = 0;
        const char *charEncoded = CodepointToUTF8(codepoint, &codepointSize);

        if ((backSpacePressed || deletePressed) && strlen(textBuffer) > 0)
        {
            textBuffer[strlen(textBuffer) - 1] = '\0';
        }
        else if(strlen(textBuffer) < bufferSize - 1 && codepoint >= 32) //can concat
        {
            strcat(textBuffer, charEncoded);
        }
        textBuffer[strlen(textBuffer)] = '\0';

    }
    else
    {
        DrawText(textBuffer, bounds.x + 5, bounds.y + (bounds.height / 2) - (textSize.y / 2), 32, GRAY);
    }
    return false;
}

bool GUI::TextButton(Rectangle bounds, const char *textToShow, Vector2 mousePos, Color buttonColor, Color borderColor)
{
    DrawRectangleLinesEx(bounds, 1.0f, borderColor);
    DrawRectangleRec(bounds, buttonColor);
    Vector2 textSize = MeasureTextEx(GetFontDefault(), textToShow, 32, 1.0f);
    if(CheckCollisionPointRec(mousePos, bounds))
    {
        DrawText(textToShow, bounds.x + (bounds.width / 2) - (textSize.x / 2), bounds.y + (bounds.height / 2) - (textSize.y / 2), 32, BLACK);
        if(IsMouseButtonDown(MOUSE_BUTTON_LEFT)) return true;
    }
    else
    {
        DrawText(textToShow, bounds.x + (bounds.width / 2) - (textSize.x / 2), bounds.y + (bounds.height / 2) - (textSize.y / 2), 32, GRAY);
    }
    return false;
}
