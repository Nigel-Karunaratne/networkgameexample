#include "networking.h"
#include <thread>
#include <mutex>

#include <csignal>

bool interrupt = true;
void handleInterrupt(int signo)
{
    std::cout << "EXITING" << std::endl;
    interrupt = false;
}

void parse_args(int argc, char const* argv[], int& maxPlayers, int& tickRate)
{
    int current_arg = 0;
    while(current_arg++ < argc)
    {
        if(argv[current_arg] == "--max-players" && current_arg++ < argc)
            maxPlayers = atoi(argv[current_arg]);
        else if(argv[current_arg] == "--tick-rate" && current_arg++ < argc)
            tickRate = atoi(argv[current_arg]);
    }

    // check bounds
    if (maxPlayers < 1 || maxPlayers > 8)
        maxPlayers = 2;
    // TODO - error checking for TickRate
}

int main(int argc, char const *argv[])
{
    int maxPlayers = 1;
    int tickRate = 2;
    parse_args(argc, argv, maxPlayers, tickRate);
    // std::signal(SIGINT, handleInterrupt); //FIXME - interrupt doesn't seem to stop any recvfrom... maybe works after threading implemented...
    Networking networking = Networking();
    networking.InitializeWinSock();
    networking.CreateServerSocket(100);

    networking.SetUpClientListening();

    std::cout << "[SERVER] Started." << std::endl;

    bool isRunning = true;
    while (isRunning && interrupt)
    {
        // networking.ListenForClients();
        // TODO - delay for simulation rate?
        networking.SendGameStateToAllPlayers();
    }

    networking.JoinAllThreads();

    return EXIT_SUCCESS;
}

// I have a cpp program that uses winsock to act like a game server. The server uses UDP. How can I handle multiple clients?

// in cpp, how would i set up a raylib game that uses client-server for networking? using udp sockets