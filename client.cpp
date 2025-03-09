#include "clientnetworking.h"
#include <iostream>

namespace rl
{
#include <raylib.h>
}

int main(void)
{
    // networking information
    char* serverIP = "127.0.0.1";
    int serverPort = 100;

    // Init Winsock
    ClientNetworking networking = ClientNetworking();
    networking.InitializeNetworking();

    networking.SetupServerSocket("127.0.0.1",100);

    const char* message = "Hello from Client!";

    rl::InitWindow(800,450, "udp client window");
    rl::SetTargetFPS(60);

    // Main game loop
    while (!rl::WindowShouldClose())
    {
        // std::cout << "start of while loop" << std::endl;
        // sendto(clientSocket, message, strlen(message), 0, (sockaddr*)&serverAddr, sizeof(serverAddr));
        networking.SendToServer(message);

        networking.ReceiveFromServer();

        // std::cout << "drawing..." << std::endl;
        rl::BeginDrawing();
                // std::cout << "A" << std::endl;
            rl::ClearBackground(rl::RAYWHITE);
                // std::cout << "B" << std::endl;
            rl::DrawText("Congrats! You created your first window!", 190, 200, 20, rl::LIGHTGRAY);
                // std::cout << "C" << std::endl;
        rl::EndDrawing();
        // std::cout << "end drawing..." << std::endl;
    }

    std::cout << "out of while loop" << std::endl;
    rl::CloseWindow();

    return 0;
}