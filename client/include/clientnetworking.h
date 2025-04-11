#pragma once

#include <string>

class ClientNetworking
{
private:
    class Impl;
    Impl* pimpl;
public:
    ClientNetworking();
    ~ClientNetworking();

    bool InitializeNetworking();
    bool SetIPAndPort(const std::string& ip, int port);
    bool SetupServerSocket();

    void SetupNetworkingThread();

    void SendToServer(const std::string& message);
    void ReceiveFromServer();

    std::string GetAddressRepresentation();

    bool HasBeenAcceptedByServer();
};

