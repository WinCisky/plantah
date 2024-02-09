#include "NetworkManager.h"
#include <iostream>
#include <string>
#include <random>
#include <sstream>

#include "Base64.h"

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

struct networkMessage
{
    bool valid;
    std::string message;
};

std::string NetworkManager::generateNonce() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 15);

    std::stringstream ss;
    for (int i = 0; i < 16; ++i) {
        ss << std::hex << dis(gen);
    }

    std::string nonce = ss.str();

    return nonce;
}

// 0                   1                   2                   3
// 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
// +-+-+-+-+-------+-+-------------+-------------------------------+
// |F|R|R|R| opcode|M| Payload len |    Extended payload length    |
// |I|S|S|S|  (4)  |A|     (7)     |             (16/64)           |
// |N|V|V|V|       |S|             |   (if payload len==126/127)   |
// | |1|2|3|       |K|             |                               |
// +-+-+-+-+-------+-+-------------+ - - - - - - - - - - - - - - - +
// |     Extended payload length continued, if payload len == 127  |
// + - - - - - - - - - - - - - - - +-------------------------------+
// |                               |Masking-key, if MASK set to 1  |
// +-------------------------------+-------------------------------+
// | Masking-key (continued)       |          Payload Data         |
// +-------------------------------- - - - - - - - - - - - - - - - +
// :                     Payload Data continued ...                :
// + - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - +
// |                     Payload Data continued ...                |
// +---------------------------------------------------------------+

std::string NetworkManager::composeWebSocketMessage(std::string & message)
{
    // Generate a random mask key
    std::vector<char> maskKey(4);
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 255);
    for (int i = 0; i < 4; ++i) {
        maskKey[i] = static_cast<char>(dis(gen));
    }

    // Apply the mask to the payload data
    for (size_t i = 0; i < message.length(); ++i) {
        message[i] = message[i] ^ maskKey[i % 4];
    }

    // Construct the WebSocket frame with the mask
    std::string frame;
    frame.push_back(0x81); // FIN bit set + Opcode for text frame
    if (message.length() <= 125) {
        frame.push_back(static_cast<char>(message.length() | 0x80)); // Mask bit set
    } else if (message.length() <= 65535) {
        frame.push_back(static_cast<unsigned char>(126 | 0x80)); // Mask bit set
        frame.push_back((message.length() >> 8) & 0xFF);
        frame.push_back(message.length() & 0xFF);
    } else {
        frame.push_back(static_cast<unsigned char>(127 | 0x80)); // Mask bit set
        for (int i = 7; i >= 0; --i) {
            frame.push_back((message.length() >> (8 * i)) & 0xFF);
        }
    }
    frame.append(maskKey.data(), maskKey.size());
    frame.append(message);
    return frame;
}

std::string NetworkManager::receiveWebSocketMessage(int clientSocket) {
    std::string msg = "";
    char buffer[1024];
    ssize_t bytesRead = recv(clientSocket, buffer, sizeof(buffer), 0);
    if (bytesRead == -1) {
        // std::cerr << "Error receiving response from server\n" << std::endl;
        return msg;
    }
    buffer[bytesRead] = '\0';

    // TODO: 
    //  - handle the case when the message is fragmented
    //  - handle the case when the message is masked
    //  - handle the case when the message is longer than 125 bytes

    // fin + rsv1 + rsv2 + rsv3 + opcode
    unsigned char firstByte = buffer[0];
    // std::cout << "First byte: " << std::hex << (int)firstByte << std::endl;
    // mask + payload length
    unsigned char secondByte = buffer[1];
    // std::cout << "Second byte: " << std::hex << (int)secondByte << std::endl;


    // print the rest of the message
    msg = buffer + 2;

    //reset incomplete message if FIN bit is set
    if (firstByte & 0x80)
    {
        m_incompleteMessage = "";
    }
    return msg;
}

NetworkManager::NetworkManager()
{
}

NetworkManager::~NetworkManager()
{
    if (m_connected) {
#ifdef WIN32
        closesocket(clientSocket);
        WSACleanup();
#else
        close(clientSocket);
#endif
    }
}

NetworkManager::NetworkManager(const std::string& address, int port)
        : m_address(address)
        , m_port(port)
{
#ifdef WIN32
    WSADATA wsaData;
    // Initialize Winsock
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "Error initializing Winsock\n";
        return;
    }
    // Create a socket
    clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == INVALID_SOCKET) {
        std::cerr << "Error creating socket\n";
        return;
    }

    // domain to ip windows
    struct addrinfo *result = NULL;
    struct addrinfo *ptr = NULL;
    struct addrinfo hints;
    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    if (getaddrinfo(m_address.c_str(), NULL, &hints, &result) != 0) {
        std::cerr << "Error getting address info\n";
        return;
    }
    for (ptr = result; ptr != NULL; ptr = ptr->ai_next) {
        struct sockaddr_in *sockaddr_ipv4 = (struct sockaddr_in *) ptr->ai_addr;
        m_address = inet_ntoa(sockaddr_ipv4->sin_addr);
        break;
    }
    freeaddrinfo(result);
#else
    // Create a socket
    clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == -1) {
        std::cerr << "Error creating socket\n";
        return;
    }

    // domain to ip
    struct hostent *he;
    struct in_addr **addr_list;
    if ((he = gethostbyname(m_address.c_str())) == NULL) {
        std::cerr << "Error getting host by name\n";
        return;
    }
    addr_list = (struct in_addr **) he->h_addr_list;
    m_address = inet_ntoa(*addr_list[0]);
    std::cout << "IP address: " << m_address << std::endl;
#endif

    // Set up server address
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(m_port);
    serverAddr.sin_addr.s_addr = inet_addr(m_address.c_str());

    // Connect to the server
    if (connect(clientSocket, (struct sockaddr *) &serverAddr, sizeof(serverAddr)) == -1) {
        std::cerr << "Error connecting to server\n";
        return;
    }

    #ifdef WIN32
        // Set the socket to non-blocking mode
        u_long mode = 1;
        if (ioctlsocket(clientSocket, FIONBIO, &mode) == SOCKET_ERROR) {
            std::cerr << "Error setting socket to non-blocking mode\n";
            return;
        }
    #else
        // Set the socket to non-blocking mode
        int flags = fcntl(clientSocket, F_GETFL, 0);
        if (flags == -1) {
            std::cerr << "Error getting socket flags\n";
            return;
        }
        if (fcntl(clientSocket, F_SETFL, flags | O_NONBLOCK) == -1) {
            std::cerr << "Error setting socket to non-blocking mode\n";
            return;
        }
    #endif

    std::string nonce = generateNonce();
    std::string base64_nonce = base64_encode(nonce.c_str(), nonce.length());

    // Send WebSocket handshake directly
    const std::string WS_HANDSHAKE = "GET / HTTP/1.1\r\n" +
                                     std::string("Host: " + m_address + "\r\n") +
                                     "Upgrade: websocket\r\n" +
                                     "Connection: Upgrade\r\n" +
                                     "Sec-WebSocket-Key: " + base64_nonce + "\r\n" +
                                     "Sec-WebSocket-Version: 13\r\n\r\n";
    if (send(clientSocket, WS_HANDSHAKE.c_str(), WS_HANDSHAKE.length(), 0) == -1) {
        std::cerr << "Error sending WebSocket handshake request\n";
        return;
    }

    auto receivedResponse = false;
    std::cout << "Waiting for response from server\n" << std::endl;
    while (!receivedResponse)
    {
        /* code */
        char buffer[1024];
        ssize_t bytesRead = recv(clientSocket, buffer, sizeof(buffer), 0);
        if (bytesRead != -1) {
            buffer[bytesRead] = '\0';
            std::cout << "Received response from server:\n" << buffer << std::endl;

            // complete the handshake
            auto response = std::string(buffer);

            if (response.find("Sec-WebSocket-Accept") != std::string::npos) {
                std::cout << "Received WebSocket handshake response\n" << std::endl;
                receivedResponse = true;
                m_connected = true;
            }
        }
    }
}

const bool NetworkManager::isConnected() const
{
    return m_connected;
}

void NetworkManager::sSend(std::string& message)
{
    std::lock_guard<std::mutex> lock(m_mutex);
    std::string frame = composeWebSocketMessage(message);
    
    // Send the WebSocket message frame
    if (send(clientSocket, frame.c_str(), frame.length(), 0) == -1) {
        std::cerr << "Error sending WebSocket message\n";
        // get error code linux
        auto error = errno;
        std::cerr << "Error code: " << error << std::endl;
        
        return;
    }
}

std::string NetworkManager::sReceive()
{
    if (!m_mutex.try_lock()) {
        // Mutex is locked, skip this operation
        return "";
    }
    std::lock_guard<std::mutex> lock(m_mutex, std::adopt_lock);
    return receiveWebSocketMessage(clientSocket);
}