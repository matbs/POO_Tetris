#include "ControllerServer.h"

ControllerServer::ControllerServer() : sock(INVALID_SOCKET), clientSock(INVALID_SOCKET), isServer(false), connected(false) {
#ifdef _WIN32
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);
#endif
    currentIP = GetLocalIP();
}

ControllerServer::~ControllerServer() {
    Close();
#ifdef _WIN32
    WSACleanup();
#endif
}

std::string ControllerServer::GetLocalIP() {
    SOCKET s = socket(AF_INET, SOCK_DGRAM, 0);
    if(s == INVALID_SOCKET) return "127.0.0.1";

    sockaddr_in google_dns;
    google_dns.sin_family = AF_INET;
    google_dns.sin_addr.s_addr = inet_addr("8.8.8.8"); 
    google_dns.sin_port = htons(53);

    if(connect(s, (sockaddr*)&google_dns, sizeof(google_dns)) == SOCKET_ERROR) {
        #ifdef _WIN32
        closesocket(s);
        #else
        close(s);
        #endif
        return "127.0.0.1";
    }

    sockaddr_in local_addr;
    #ifdef _WIN32
    int addr_len = sizeof(local_addr);
    #else
    socklen_t addr_len = sizeof(local_addr);
    #endif
    
    getsockname(s, (sockaddr*)&local_addr, &addr_len);

    #ifdef _WIN32
    closesocket(s);
    #else
    close(s);
    #endif

    return std::string(inet_ntoa(local_addr.sin_addr));
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
    sock = INVALID_SOCKET;
    clientSock = INVALID_SOCKET;
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
    
    if (sock == INVALID_SOCKET) return false;

    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(port);

    if (bind(sock, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        return false;
    }
    
    if (listen(sock, 1) == SOCKET_ERROR) {
        return false;
    }
    
    SetNonBlocking(sock);
    return true;
}

bool ControllerServer::ConnectClient(const std::string& ip, int port) {
    isServer = false;
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == INVALID_SOCKET) return false;

    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port);

    if (inet_pton(AF_INET, ip.c_str(), &serverAddr.sin_addr) <= 0) {
        return false;
    }

    if (connect(sock, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        Close();
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