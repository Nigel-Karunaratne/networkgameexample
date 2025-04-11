#include "winsock2.h"
#include "clientnetworking.h"

#include <iostream>
#include <thread>

class ClientNetworking::Impl
{
private:
    WSAData wsaData;
    SOCKET clientSocket;
    sockaddr_in serverAddr;

    int serverPort;
    std::string serverIP;

    std::thread networkingThread;

    bool acceptedInGame = false;
public:

    ~Impl();
    bool InitializeNetworking();
    bool SetupServerSocket();
    bool SetIPAndPort(const std::string& ip, int port);

    void SendToServer(const std::string& message);
    void ReceiveFromServer();

    void SetupNetworkingThread();

    std::string GetAddressRepresentation();

    bool HasBeenAcceptedByServer();
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

bool ClientNetworking::Impl::SetIPAndPort(const std::string& ip, int port)
{
    // TODO - validate INPUT!
    serverIP = ip;
    serverPort = port;
    return true;
}

bool ClientNetworking::Impl::SetupServerSocket()
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

void ClientNetworking::Impl::SetupNetworkingThread()
{
    std::cout << GetAddressRepresentation() << std::endl;
    SetupServerSocket();
    std::cout << "SOCKET SET UP" << std::endl;
    this->networkingThread = std::thread(&ClientNetworking::Impl::ReceiveFromServer, this);
    std::cout << "THREAD SET UP" << std::endl;
    
    SendToServer("459 CONN");
}

void ClientNetworking::Impl::SendToServer(const std::string &message)
{
    // std::cout << "Sending " << message << " to server" << std::endl;
    int bytes_send = sendto(clientSocket, message.c_str(), message.length(), 0, (sockaddr*)&serverAddr, sizeof(serverAddr));
    std::cout << "send: " << bytes_send << std::endl;
}

void ClientNetworking::Impl::ReceiveFromServer()
{
    char buffer[1024];

    sockaddr_in fromAddr;
    int fromAddrLen = sizeof(fromAddr);
    
    while(true)
    {
        int bytesReceived = recvfrom(clientSocket, buffer, sizeof(buffer), 0, (sockaddr*)&fromAddr, &fromAddrLen);
        if (bytesReceived > 0)
        {
            buffer[bytesReceived] = '\0'; // Null-terminate the received message
            std::cout << "Received from server: " << buffer << std::endl;

            std::string msg(buffer);
            if(!acceptedInGame && msg.substr(0,3) == "159")
            {
                acceptedInGame = true;
            }
        }
        else
        {
            // std::cout << "GOT NOTHING?" << std::endl;
        }
    }
}

std::string ClientNetworking::Impl::GetAddressRepresentation()
{
    return serverIP + ":" + std::to_string(serverPort);
}

bool ClientNetworking::Impl::HasBeenAcceptedByServer()
{
    return acceptedInGame;
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

bool ClientNetworking::SetIPAndPort(const std::string &ip, int port)
{
    return pimpl->SetIPAndPort(ip, port);
}

bool ClientNetworking::SetupServerSocket()
{
    return pimpl->SetupServerSocket();
}

void ClientNetworking::SendToServer(const std::string& message)
{
    return pimpl->SendToServer(message);
}

void ClientNetworking::ReceiveFromServer()
{
    pimpl->ReceiveFromServer();
}

std::string ClientNetworking::GetAddressRepresentation()
{
    return pimpl->GetAddressRepresentation();
}

void ClientNetworking::SetupNetworkingThread()
{
    pimpl->SetupNetworkingThread();
}

bool ClientNetworking::HasBeenAcceptedByServer()
{
    return pimpl->HasBeenAcceptedByServer();
}