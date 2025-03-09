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
    bool SetupServerSocket(const std::string& ip, int port);

    void SendToServer(const std::string& message);
    void ReceiveFromServer();
};

