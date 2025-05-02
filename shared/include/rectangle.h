#pragma once

typedef struct GameRectangle {
    int x;
    int y;
    int width;
    int height;
    
    bool intersects(const GameRectangle& other) const {
        return !(x + width < other.x || x > other.x + other.width ||
                 y + height < other.y || y > other.y + other.height);
    }
} GameRectangle;
