#include <queue>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include <thread>
#include "Tetrominos.h"

struct GamePacket {
    int board[20][10];
    tetromino nextTetromino;
    int linesCleared;
    int score;
    int lifes;
    bool gameOver;
};

class ThreadSafeQueue {
private:
    std::queue<GamePacket> queue;
    std::mutex mtx;
    std::condition_variable cv;
    std::atomic<bool> shutdown;

public:
    ThreadSafeQueue() : shutdown(false) {};
    
    void push(const GamePacket& packet);

    bool pop(GamePacket& packet);

    bool try_pop(GamePacket& packet);
    
    void shutdown_queue();
};