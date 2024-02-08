// NetworkManager.h
#pragma once

#include <string>
#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

const std::string SERVER_IP = "127.0.0.1";
const int SERVER_PORT = 8080;

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