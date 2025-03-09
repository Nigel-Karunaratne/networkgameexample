#include "networking.h"

int main(int argc, char const *argv[])
{
    Networking networking = Networking();
    networking.InitializeWinSock();
    networking.CreateServerSocket(100);

    bool isRunning = true;
    while (isRunning)
    {
        networking.HandleIncomingData();
    }
    
    return EXIT_SUCCESS;
}


// in cpp, how would i set up a raylib game that uses client-server for networking? using udp sockets