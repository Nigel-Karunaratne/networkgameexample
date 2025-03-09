#pragma once

#include <winsock2.h>
#include <iostream>

class Networking
{
private:
    WSAData wsaData;

    int port;
    SOCKET serverSocket;
public:
    Networking() {};
    ~Networking() { WSACleanup(); };

    bool InitializeWinSock();
    bool CreateServerSocket(int port);

    void HandleIncomingData();
};

inline bool Networking::InitializeWinSock()
{
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "Winsock initialization failed." << std::endl;
        return false;
    }
    return true;
}

inline bool Networking::CreateServerSocket(int port)
{
    serverSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if(serverSocket == INVALID_SOCKET)
    {
        std::cerr << "Failed to create server socket." << std::endl;
        return false;
    }

    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(port);

    if (bind(serverSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        std::cerr << "Failed to bind server socket." << std::endl;
        return false;
    }
    else
    {
        std::cout << serverAddr.sin_addr.s_addr << std::endl;
    }

    return true;
}

inline void Networking::HandleIncomingData()
{
    char buffer[512];
    sockaddr_in clientAddr;
    int clientAddrLen = sizeof(clientAddr);

    int bytesReceived = recvfrom(serverSocket, buffer, sizeof(buffer), 0, (sockaddr*)&clientAddr, &clientAddrLen);
    if(bytesReceived > 0)
    {
        buffer[bytesReceived] = '\0';
        std::cout << "Received message: " << buffer << std::endl;

        const char* response = "Message received!";
        sendto(serverSocket, response, strlen(response), 0, (sockaddr*)&clientAddr, clientAddrLen);
    }
}
