// USES RAYLIB 5.5, RAYGUI 4.0

#include "clientnetworking.h"
#include "clientstate.h"
#include "screen_title.h"


#include <iostream>
#include <thread>
#include "raylib.h"
#include "raymath.h"

#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 400

#define MIN(a, b) ((a)<(b) ? (a) : (b))
#define MAX(a, b) ((a)>(b) ? (a) : (b))

int main(void)
{
    ClientState state = ClientState::STATE_TITLE;
    TitleScreen titleScreen = TitleScreen(); 

    // Init Networking
    ClientNetworking networking = ClientNetworking();
    networking.InitializeNetworking();
    networking.SetupServerSocket("127.0.0.1",100);

    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "udp client window");
    SetTargetFPS(60);

    std::thread networkingThread(&ClientNetworking::ReceiveFromServer, networking);

    networking.SendToServer("A new client wants to connect!");

    RenderTexture2D renderTexture = LoadRenderTexture(SCREEN_WIDTH, SCREEN_HEIGHT);
    SetTextureFilter(renderTexture.texture, TEXTURE_FILTER_BILINEAR);  // Texture scale filter to use

    Vector2 virtualMousePos = (Vector2){0,0};

    // Main game loop
    while (!WindowShouldClose())
    {
        // Framebuffer Scaling and "Virtual Mouse Pos"
        float scale = MIN((float)GetScreenWidth()/SCREEN_WIDTH, (float)GetScreenHeight()/SCREEN_HEIGHT);
        Vector2 realMousePos = GetMousePosition();
        virtualMousePos.x = (realMousePos.x - (GetScreenWidth() - (SCREEN_WIDTH*scale))*0.5f)/scale;
        virtualMousePos.y = (realMousePos.y - (GetScreenHeight() - (SCREEN_HEIGHT*scale))*0.5f)/scale;
        virtualMousePos = Vector2Clamp(virtualMousePos, (Vector2){ 0, 0 }, (Vector2){ (float)SCREEN_WIDTH, (float)SCREEN_HEIGHT });
        
        switch(state)
        {
            case ClientState::STATE_TITLE:
                titleScreen.Update(state);
                titleScreen.Draw(renderTexture, virtualMousePos);
                break;
            case ClientState::STATE_NETWORKGAME:
                break;
            case ClientState::STATE_DISCONNECT:
                break;
            default:
                break;
        }


        //Draw framebuffer texture to Window (see raylib example core_window_letterbox.c)
        BeginDrawing();
            ClearBackground(BLACK);
            DrawTexturePro(renderTexture.texture, (Rectangle){ 0.0f, 0.0f, (float)renderTexture.texture.width, (float)-renderTexture.texture.height },
            (Rectangle){ (GetScreenWidth() - ((float)SCREEN_WIDTH*scale))*0.5f, (GetScreenHeight() - ((float)SCREEN_HEIGHT*scale))*0.5f,
            (float)SCREEN_WIDTH*scale, (float)SCREEN_HEIGHT*scale }, (Vector2){ 0, 0 }, 0.0f, WHITE);
        EndDrawing();
    }

    std::cout << "out of while loop" << std::endl;
    CloseWindow();

    return 0;
}
