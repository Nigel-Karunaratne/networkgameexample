#pragma once

#include <string>
#include "raylib.h"

class GUI
{
public:
    // PLACE IN A DRAW LOOP, AS THESE ASSUME DRAWING IS ACTIVE
    static bool TextBox(Rectangle bounds, char* textBuffer, int bufferSize, Vector2 mousePos);
    static bool TextButton(Rectangle bounds, const char* textToShow, Vector2 mousePos, Color buttonColor, Color borderColor);
};
