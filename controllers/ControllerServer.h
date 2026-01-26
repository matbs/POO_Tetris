#pragma once

#include <vector>
#include <string>
#include <cstring>

#include "../models/Tetrominos.h"

#ifdef _WIN32
    #include <winsock2.h>
    #include <ws2tcpip.h>
    #pragma comment(lib, "ws2_32.lib")
#else
    #include <sys/socket.h>
    #include <arpa/inet.h>
    #include <unistd.h>
    #include <fcntl.h>
    #define INVALID_SOCKET -1
    #define SOCKET_ERROR -1
    typedef int SOCKET;
#endif

struct GamePacket {
    int board[20][10];
    tetromino nextTetromino;
    int linesCleared;
    int score;
    int lifes;
    bool gameOver;
};

class ControllerServer {
private:
    SOCKET sock;
    SOCKET clientSock;
    bool isServer;
    bool connected;

public:
    ControllerServer();
    ~ControllerServer();

    bool InitServer(int port);
    bool ConnectClient(const std::string& ip, int port);
    
    void SendPacket(const GamePacket& packet);
    
    bool ReceivePacket(GamePacket& packet);

    bool IsConnected() const { return connected; }
    void Close();
};