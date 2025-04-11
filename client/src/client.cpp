// USES RAYLIB 5.5, RAYGUI 4.0

#include "clientnetworking.h"
#include "clientstate.h"

#include "screen.h"
#include "screen_title.h"
#include "screen_connecting.h"

#include <iostream>
#include <thread>
#include "raylib.h"
#include "raymath.h"

#define SCREEN_HEIGHT 720
#define SCREEN_WIDTH 1280

#define MIN(a, b) ((a)<(b) ? (a) : (b))
#define MAX(a, b) ((a)>(b) ? (a) : (b))

int main(void)
{
    ClientState state = ClientState::STATE_TITLE;
    
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "udp client window");
    SetTargetFPS(60);
    
    // Init Networking
    ClientNetworking networking = ClientNetworking();
    if (!networking.InitializeNetworking())
    {
        std::cerr << "Could not initialize networking!" << std::endl;
        return 1;
    }
    
    Screen* currentScreen = new TitleScreen(networking);

    RenderTexture2D renderTexture = LoadRenderTexture(SCREEN_WIDTH, SCREEN_HEIGHT);
    SetTextureFilter(renderTexture.texture, TEXTURE_FILTER_BILINEAR);  // Texture scale filter to use

    Vector2 virtualMousePos = (Vector2){0,0};

    std::thread networkingThread;

    // Main game loop
    while (!WindowShouldClose())
    {
        // Framebuffer Scaling and "Virtual Mouse Pos"
        float scale = MIN((float)GetScreenWidth()/SCREEN_WIDTH, (float)GetScreenHeight()/SCREEN_HEIGHT);
        Vector2 realMousePos = GetMousePosition();
        virtualMousePos.x = (realMousePos.x - (GetScreenWidth() - (SCREEN_WIDTH*scale))*0.5f)/scale;
        virtualMousePos.y = (realMousePos.y - (GetScreenHeight() - (SCREEN_HEIGHT*scale))*0.5f)/scale;
        virtualMousePos = Vector2Clamp(virtualMousePos, (Vector2){ 0, 0 }, (Vector2){ (float)SCREEN_WIDTH, (float)SCREEN_HEIGHT });
        
        Screen* newScreen = currentScreen->Update(state);
        if (newScreen != nullptr)
        {
            currentScreen = newScreen;
        }
        
        currentScreen->Draw(renderTexture, virtualMousePos);

        //Draw framebuffer texture to Window (see raylib example core_window_letterbox.c)
        BeginDrawing();
            ClearBackground(BLACK);
            DrawTexturePro(renderTexture.texture, (Rectangle){ 0.0f, 0.0f, (float)renderTexture.texture.width, (float)-renderTexture.texture.height },
            (Rectangle){ (GetScreenWidth() - ((float)SCREEN_WIDTH*scale))*0.5f, (GetScreenHeight() - ((float)SCREEN_HEIGHT*scale))*0.5f,
            (float)SCREEN_WIDTH*scale, (float)SCREEN_HEIGHT*scale }, (Vector2){ 0, 0 }, 0.0f, WHITE);
        EndDrawing();

        // std::cout << state << std::endl;
    }

    std::cout << "out of while loop" << std::endl;
    CloseWindow();

    return 0;
}
