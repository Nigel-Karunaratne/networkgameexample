#include "winsock2.h"
#include "clientnetworking.h"

#include <iostream>
#include <thread>
#include <atomic>

#include "protocol.h"

class ClientNetworking::Impl
{
private:
    WSAData wsaData;
    SOCKET clientSocket;
    sockaddr_in serverAddr;

    int serverPort;
    std::string serverIP;

    std::thread networkingThread;

    int acceptedInGameStatus = 0;

    std::atomic<bool> threadRunning;
public:

    ~Impl();
    bool InitializeNetworking();
    bool SetupServerSocket();
    bool SetIPAndPort(const std::string& ip, int port);

    void sendToServer(const std::string& message);
    void receiveFromServer_Thread();

    bool SetupNetworkingThread();
    void ShutdownNetworkngThread();

    std::string GetAddressRepresentation();

    int GetConnectionRequestStatus();
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

    // WINDOWS-SPECIFIC
    u_long mode = 1;
    ioctlsocket(clientSocket, FIONBIO, &mode);

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

bool ClientNetworking::Impl::SetupNetworkingThread()
{
    std::cout << GetAddressRepresentation() << std::endl;
    bool result = SetupServerSocket();
    if(!result)
        return false;
    
    std::cout << "SOCKET SET UP" << std::endl;
    
    threadRunning = true;
    this->networkingThread = std::thread(&ClientNetworking::Impl::receiveFromServer_Thread, this);
    std::cout << "THREAD SET UP" << std::endl;
    
    sendToServer(protocol::CLIENT_CONNECT_REQUEST);

    return true;
}

void ClientNetworking::Impl::ShutdownNetworkngThread()
{
    threadRunning = false;
    if(networkingThread.joinable())
        this->networkingThread.detach();
}

void ClientNetworking::Impl::sendToServer(const std::string &message)
{
    // std::cout << "Sending " << message << " to server" << std::endl;
    int bytes_send = sendto(clientSocket, message.c_str(), message.length(), 0, (sockaddr*)&serverAddr, sizeof(serverAddr));
    std::cout << "send: " << bytes_send << std::endl;
}

void ClientNetworking::Impl::receiveFromServer_Thread()
{
    char buffer[1024];

    sockaddr_in fromAddr;
    int fromAddrLen = sizeof(fromAddr);
    
    while(threadRunning)
    {
        // FOR UNIX, int bytesReceived = recvfrom(clientSocket, buffer, sizeof(buffer), MSG_DONTWAIT, (sockaddr*)&fromAddr, &fromAddrLen);
        int bytesReceived = recvfrom(clientSocket, buffer, sizeof(buffer), 0, (sockaddr*)&fromAddr, &fromAddrLen);
        if (bytesReceived > 0)
        {
            buffer[bytesReceived] = '\0'; //null-terminate the received message
            std::cout << "Received from server: " << buffer << std::endl;

            std::string msg(buffer);
            if(acceptedInGameStatus == 0)
            {
                if (msg.substr(0,3) == "159")
                {
                    acceptedInGameStatus = 1;
                }
                else if (msg.substr(0,3) == "000")
                {
                    std::cout << "rejected" << std::endl;
                    acceptedInGameStatus = -1;
                }
            }
        }
        else
        {
            // std::cout << "GOT NOTHING?" << std::endl;
        }
    }
    std::cout << "DONE!!!!!!!!!!!!!!!!!!!!" << std::endl;
}

std::string ClientNetworking::Impl::GetAddressRepresentation()
{
    return serverIP + ":" + std::to_string(serverPort);
}

int ClientNetworking::Impl::GetConnectionRequestStatus()
{
    return acceptedInGameStatus;
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

std::string ClientNetworking::GetAddressRepresentation()
{
    return pimpl->GetAddressRepresentation();
}

bool ClientNetworking::SetupNetworkingThread()
{
    return pimpl->SetupNetworkingThread();
}

void ClientNetworking::ShutdownNetworkngThread()
{
    std::cout << "    SHUTTING DOWN THREAD..."<<std::endl;
    pimpl->ShutdownNetworkngThread();
}

int ClientNetworking::GetConnectionRequestStatus()
{
    return pimpl->GetConnectionRequestStatus();
}