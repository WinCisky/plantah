// NetworkManager.h
#pragma once

#include <SFML/Network.hpp>

#include <string>
#include <iostream>

class NetworkManager {
    sf::TcpSocket m_socket;
    std::string m_address;
    int m_port;
    bool m_connected = false;
    
public:
    NetworkManager() = default;

    NetworkManager(const std::string& address, int port)
        : m_address(address)
        , m_port(port)
    {
        auto addressResolved = sf::IpAddress::resolve(m_address);
        std::cout << "Resolved address: " << addressResolved.value() << std::endl;
        auto status = m_socket.connect(sf::IpAddress::resolve(address).value(), m_port);
        if (status != sf::Socket::Status::Done)
        {
            std::cout << "Error connecting to server" << std::endl;
        }
        std::cout << "Connected to server" << std::endl;
        m_socket.setBlocking(false);
        m_connected = true;
    }

    const bool isConnected() const
    {
        return m_connected;
    }

    void send(const std::string& message)
    {
        if (m_connected)
        {
            auto status = m_socket.send(message.c_str(), message.size());
            if (status != sf::Socket::Status::Done)
            {
                std::cout << "Error sending message" << std::endl;
            }
        }
    }

    std::string receive()
    {
        if (m_connected)
        {
            char buffer[1024];
            std::size_t received;
            auto status = m_socket.receive(buffer, sizeof(buffer), received);
            // not ready means no message to receive
            if (status == sf::Socket::Status::NotReady)
            {
                return "";
            }
            // done means message received
            if (status != sf::Socket::Status::Done)
            {
                std::cout << "Error receiving message" << std::endl;
            }
            return std::string(buffer, received);
        }
        return "";
    }

};