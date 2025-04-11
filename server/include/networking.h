#pragma once

#include "protocol.h"

#include <winsock2.h>
#include <iostream>
#include <unordered_map>
#include <string>
#include <thread>
#include <mutex>
#include <vector>

struct Client
{
    sockaddr_in address;

    std::string getKey() { return inet_ntoa(this->address.sin_addr) + std::to_string(ntohs(this->address.sin_port)); }
};

class Networking
{
private:
    WSAData wsaData;
    int port;
    SOCKET serverSocket;

    std::unordered_map<std::string, Client> clientMap;

    std::vector<std::thread> allThreads;
    std::mutex clientThreadMutex;

    int maxClients;

    void CreateNewPlayer(std::string key, Client newClient);
    void HandleClient_Thread(SOCKET serverSocket, Client client);
    void ListenForClients_Thread();
public:
    Networking(int maxClients) : maxClients(maxClients) {};
    ~Networking() { closesocket(serverSocket); WSACleanup(); };

    bool InitializeWinSock();
    bool CreateServerSocket(int port);

    void SetUpClientListening();

    void SendGameStateToAllPlayers();

    void ShutdownServer();
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
        // std::cout << serverAddr.sin_addr.s_addr << std::endl;
    }

    return true;
}

inline void Networking::SetUpClientListening()
{
    allThreads.push_back(std::thread(&ListenForClients_Thread, this));
    //TODO - replace with std::move?
}

inline void Networking::ListenForClients_Thread()
{
    char buffer[512];
    sockaddr_in clientAddr;
    int clientAddrLen = sizeof(clientAddr);

    while (true)
    {
        int bytesReceived = recvfrom(serverSocket, buffer, sizeof(buffer), 0, (sockaddr*)&clientAddr, &clientAddrLen);
        if(bytesReceived > 0)
        {
            std::string key = inet_ntoa(clientAddr.sin_addr) + std::to_string(ntohs(clientAddr.sin_port));
            if (clientMap.find(key) == clientMap.end())
            {
                if(clientMap.size() >= maxClients)
                {
                    // reject
                    sendto(serverSocket, protocol::SERVER_CONNECT_REJECT, protocol::SERVER_CONNECT_REJECT_LEN, 0, (sockaddr*)&clientAddr, clientAddrLen);
                }
                else //accept
                {
                    CreateNewPlayer(key, (Client){clientAddr});
                    sendto(serverSocket, protocol::SERVER_CONNECT_ACCEPT, protocol::SERVER_CONNECT_ACCEPT_LEN, 0, (sockaddr*)&clientAddr, clientAddrLen);
                }
            }
            else
            {
                // Existing client -- do nothing, right?
            }
            
            buffer[bytesReceived] = '\0';
            // std::cout << "Received message: " << buffer << std::endl;

            // const char* response = "ACK";
            // sendto(serverSocket, response, strlen(response), 0, (sockaddr*)&clientAddr, clientAddrLen);
        }
    }
}

inline void Networking::SendGameStateToAllPlayers()
{
    for(auto& clientPair: clientMap)
    {
        Client& c = clientPair.second;

        // TODO - serialize game data
        const char *buffer = "this is the game state"; // char buffer[10];

        // TODO - send to client
        sendto(serverSocket, buffer, strlen(buffer), 0, (sockaddr*)&c.address, sizeof(c.address));
    }
}

inline void Networking::ShutdownServer()
{
    std::cout << "[SERVER] Shutting down..." << std::endl;
    {
        std::lock_guard<std::mutex> lock(clientThreadMutex);
        for (auto& thread : allThreads) {
            if (thread.joinable()) {
                std::cout << "  Joining thread: " << thread.get_id() << std::endl;
                thread.join();
                std::cout << "  Thread: " << thread.get_id() << " joined." << std::endl;
            } else {
                std::cout << "  Thread: " << thread.get_id() << " is not joinable." << std::endl;
            }
        }
        allThreads.clear();
    }
    std::cout << "[SERVER] Shutdown Complete." << std::endl;
}

inline void Networking::CreateNewPlayer(std::string key, Client newClient)
{
    std::cout << "NEW CLIENT: " << key << std::endl;
    clientMap[key] = newClient;
    allThreads.push_back(std::thread(&HandleClient_Thread, this, serverSocket, newClient));
}

inline void Networking::HandleClient_Thread(SOCKET serverSocket, Client client)
{
    char buffer[512];
    int addrSize = sizeof(client.address);

    
    while(true)
    {
        // WINDOWS SPECIFIC!!
        // Set up the fd_set for select()
        fd_set readfds;
        FD_ZERO(&readfds);  // Clear the fd_set
        FD_SET(serverSocket, &readfds);  // Add the UDP socket to the fd_set
    
        // Set the timeout (in seconds)
        struct timeval timeout;
        timeout.tv_sec = 5;
        timeout.tv_usec = 0;
        
        int result = select(serverSocket + 1, &readfds, NULL, NULL, &timeout);
        if (result == -1)
        {
            perror("select() failed");
            break;
        }
        else if (result == 0)
        {
            // Timeout occurred
            std::cout << "Timeout occurred, no data received within " << 5 << " seconds from " << inet_ntoa(client.address.sin_addr) << ":" << ntohs(client.address.sin_port) << std::endl;            
            break; // TODO - instead of just exiting loop, remove client and kill this thread! REMOVE FROM GAME STATE!
        }
        else
        {
            // Data available, read it
            if (FD_ISSET(serverSocket, &readfds))
            {
                int len = recvfrom(serverSocket, buffer, sizeof(buffer), 0, (struct sockaddr*)&client.address, &addrSize);
                if (len == -1)
                {
                    perror("recvfrom failed");
                    break;
                }
                else
                {
                    std::cout << "Received " << len << " bytes from client: " << inet_ntoa(client.address.sin_addr) << ":" << ntohs(client.address.sin_port) << std::endl;
                    // Process received data here...
                }
            }
        }

        // int bytesReceived = recvfrom(serverSocket, buffer, sizeof(buffer), 0, (sockaddr*)&client.address, &addrSize);
        // if(bytesReceived > 0)
        // {
        //     std::lock_guard<std::mutex> lock(clientThreadMutex);
        //     // TODO -- update a game state here?
        //     std::cout << "game state updated..." << std::endl;
        // }
    }

    // out of while, remove this client and thread
    std::lock_guard<std::mutex> lock(clientThreadMutex); //lock gets destroyed when out of scope
    std::string key = client.getKey();
    if(clientMap.find(key) == clientMap.end())
    {
        // NOT IN -- error!
    }
    else
    {
        // TODO - send final "disconnect" message?
        clientMap.erase(key);
    }

    return;
}
