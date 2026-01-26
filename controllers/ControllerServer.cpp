#include "ControllerServer.h"
#include <iostream>

ControllerServer::ControllerServer() : sock(INVALID_SOCKET), clientSock(INVALID_SOCKET), isServer(false), connected(false) {
#ifdef _WIN32
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);
#endif
}

ControllerServer::~ControllerServer() {
    Close();
#ifdef _WIN32
    WSACleanup();
#endif
}

void ControllerServer::Close() {
    if (sock != INVALID_SOCKET) {
#ifdef _WIN32
        closesocket(sock);
        if (clientSock != INVALID_SOCKET) closesocket(clientSock);
#else
        close(sock);
        if (clientSock != -1) close(clientSock);
#endif
    }
    connected = false;
}

void SetNonBlocking(SOCKET s) {
#ifdef _WIN32
    u_long mode = 1;
    ioctlsocket(s, FIONBIO, &mode);
#else
    int flags = fcntl(s, F_GETFL, 0);
    fcntl(s, F_SETFL, flags | O_NONBLOCK);
#endif
}

bool ControllerServer::InitServer(int port) {
    isServer = true;
    sock = socket(AF_INET, SOCK_STREAM, 0);
    
    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(port);

    if (bind(sock, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) return false;
    listen(sock, 1);
    
    SetNonBlocking(sock);
    return true;
}

bool ControllerServer::ConnectClient(const std::string& ip, int port) {
    isServer = false;
    sock = socket(AF_INET, SOCK_STREAM, 0);

    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port);
    inet_pton(AF_INET, ip.c_str(), &serverAddr.sin_addr);

    if (connect(sock, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        return false;
    }

    connected = true;
    SetNonBlocking(sock);
    return true;
}

void ControllerServer::SendPacket(const GamePacket& packet) {
    SOCKET target = isServer ? clientSock : sock;
    if (target == INVALID_SOCKET) return;

    send(target, (char*)&packet, sizeof(GamePacket), 0);
}

bool ControllerServer::ReceivePacket(GamePacket& packet) {
    if (isServer && !connected) {
        struct sockaddr_in clientAddr;
        socklen_t clientLen = sizeof(clientAddr);
        SOCKET tempSock = accept(sock, (struct sockaddr*)&clientAddr, &clientLen);
        
        if (tempSock != INVALID_SOCKET) {
            clientSock = tempSock;
            connected = true;
            SetNonBlocking(clientSock);
        }
    }

    SOCKET target = isServer ? clientSock : sock;
    if (target == INVALID_SOCKET) return false;

    int bytesReceived = recv(target, (char*)&packet, sizeof(GamePacket), 0);
    return bytesReceived > 0;
}