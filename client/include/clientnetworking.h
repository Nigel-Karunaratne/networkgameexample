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

    bool SetupNetworkingThread();
    void ShutdownNetworkngThread();

    std::string GetAddressRepresentation();

    int GetConnectionRequestStatus(); //-1 for reject, 1 for accept, 0 for nothing;
};

