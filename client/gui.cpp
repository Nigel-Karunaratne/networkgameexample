#include "gui.h"
#include "string.h"

bool GUI::TextBox(Rectangle bounds, char *textBuffer, Vector2 mousePos)
{
    DrawRectangleLinesEx(bounds, 1.0f, BLACK);
    Vector2 textSize = MeasureTextEx(GetFontDefault(), textBuffer, 32, 1.0f);

    if(CheckCollisionPointRec(mousePos, bounds))
    {
        DrawText(textBuffer, bounds.x + 5, bounds.y + (textSize.y / 2), 32, BLACK);

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
        else if(strlen(textBuffer) < sizeof(textBuffer) && codepoint >= 32) //can concat
        {
            strcat(textBuffer, charEncoded);
        }
        textBuffer[strlen(textBuffer)] = '\0';

    }
    else
    {
        DrawText(textBuffer, bounds.x + 5, bounds.y + (textSize.y / 2), 32, GRAY);
    }
    return false;
}