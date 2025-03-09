#include "winsock2.h"
#include "clientnetworking.h"

#include <iostream>

class ClientNetworking::Impl
{
private:
    WSAData wsaData;
    SOCKET clientSocket;
    sockaddr_in serverAddr;

    int serverPort;
    std::string serverIP;
public:

    ~Impl();
    bool InitializeNetworking();
    bool SetupServerSocket(const std::string& ip, int port);

    void SendToServer(const std::string& message);
    void ReceiveFromServer();
};

ClientNetworking::Impl::~Impl()
{
    WSACleanup();
}

bool ClientNetworking::Impl::InitializeNetworking()
{
    int stat = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (stat != 0) {
        std::cerr << stat << std::endl;
        std::cerr << "Winsock initialization failed." << std::endl;
        return false;
    }
    return true;
}

bool ClientNetworking::Impl::SetupServerSocket(const std::string &ip, int port)
{
    clientSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (clientSocket == INVALID_SOCKET) {
        std::cerr << "Failed to create socket." << std::endl;
        WSACleanup();
        return false;
    }

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(serverPort);
    serverAddr.sin_addr.s_addr = inet_addr(serverIP.c_str());

    if (serverAddr.sin_addr.s_addr == INADDR_NONE) {
        std::cerr << "Invalid server IP address!" << std::endl;
        WSACleanup();
        return false;
    }
    return true;
}

void ClientNetworking::Impl::SendToServer(const std::string &message)
{
    std::cout << "Sending " << message << " to server" << std::endl;
    sendto(clientSocket, message.c_str(), message.length(), 0, (sockaddr*)&serverAddr, sizeof(serverAddr));
}

void ClientNetworking::Impl::ReceiveFromServer()
{
    char buffer[1024];

    sockaddr_in fromAddr;
    int fromAddrLen = sizeof(fromAddr);
    int bytesReceived = recvfrom(clientSocket, buffer, sizeof(buffer), 0, (sockaddr*)&fromAddr, &fromAddrLen);

    if (bytesReceived > 0) {
        buffer[bytesReceived] = '\0'; // Null-terminate the received message
        std::cout << "Received from server: " << buffer << std::endl;
    }
    else
    {
        // std::cout << "GOT NOTHING?" << std::endl;
    }
}

ClientNetworking::ClientNetworking() : pimpl(new Impl())
{
}

ClientNetworking::~ClientNetworking()
{
    delete pimpl;
}

bool ClientNetworking::InitializeNetworking()
{
    return pimpl->InitializeNetworking();
}

bool ClientNetworking::SetupServerSocket(const std::string &ip, int port)
{
    return pimpl->SetupServerSocket(ip, port);
}

void ClientNetworking::SendToServer(const std::string& message)
{
    return pimpl->SendToServer(message);
}

void ClientNetworking::ReceiveFromServer()
{
    pimpl->ReceiveFromServer();
}
