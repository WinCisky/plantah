// NetworkManager.h
#pragma once

#include <string>
#include <iostream>
#include <mutex>

#ifdef WIN32
    #pragma comment(lib, "ws2_32.lib")
    #include <winsock2.h>
    #include <ws2tcpip.h>
#else
    #include <sys/socket.h>
    #include <netdb.h>
    #include <arpa/inet.h>
    #include <unistd.h>
    #include <fcntl.h>
#endif

class NetworkManager {
    std::string m_address;
    std::string m_incompleteMessage;
    int m_port;
    bool m_connected = false;
    std::mutex m_mutex;

    int clientSocket;
    struct sockaddr_in serverAddr;

    std::string generateNonce();
    std::string composeWebSocketMessage(std::string & message);
    std::string receiveWebSocketMessage(int clientSocket);
    
public:
    NetworkManager();
    ~NetworkManager();
    NetworkManager(const std::string& address, int port);

    const bool isConnected() const;
    void sSend(std::string& message);
    std::string sReceive();

};