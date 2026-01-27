#pragma once

#define _WINSOCK_DEPRECATED_NO_WARNINGS 
#define _CRT_SECURE_NO_WARNINGS
#define WIN32_LEAN_AND_MEAN
#define NOGDI              
#define NOUSER

#include <iostream>
#include <cstring>

#include <vector>
#include <cstring>

#include "../models/ThreadQueue.h"

#ifdef _WIN32
    #include <winsock2.h>
    #include <ws2tcpip.h>
    #pragma comment(lib, "ws2_32.lib")
#else
    #include <sys/types.h>
    #include <sys/socket.h>
    #include <netinet/in.h>
    #include <sys/socket.h>
    #include <arpa/inet.h>
    #include <unistd.h>
    #include <fcntl.h>
    #include <netdb.h>
    #define INVALID_SOCKET -1
    #define SOCKET_ERROR -1
    typedef int SOCKET;
#endif

class ControllerServer {
private:
    SOCKET sock;
    SOCKET clientSock;
    bool isServer;
    bool connected;

    std::string currentIP;

    std::string GetLocalIP();

    ThreadSafeQueue incomingQueue;
    ThreadSafeQueue outgoingQueue;

    std::thread receiverThread;
    std::thread senderThread;
    std::thread accepterThread;

    std::atomic<bool> running;
    
    void receiverWorker();
    void senderWorker();
    void accepterWorker();

public:
    ControllerServer();
    ~ControllerServer();

    bool InitializeSocket(int port);
    bool InitServer(int port);
    bool ConnectClient(const std::string& ip, int port);
    bool ConnectToServer(const std::string& ip, int port); 

    void SendPacket(const GamePacket& packet);
    
    bool PollPacket(GamePacket& packet);

    std::string GetCurrentIP() const { return currentIP; }

    bool IsConnected() const { return connected; }
    void Close();
};