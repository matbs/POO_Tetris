#include "ControllerTetris.h"

#include <iostream>

ControllerTetris::ControllerTetris() {
    for(int i=0;i<20;++i)
        for(int j=0;j<10;++j)
            boardCells[i][j]=0;
    
    this->spawnTetromino();
}

ControllerTetris::~ControllerTetris() {}

void ControllerTetris::GameLoop() {
    const double interval = 0.5;
    double lastUpdateTime = GetTime();

    while (this->running)
    {
        double currentTime = GetTime();
        if (currentTime - lastUpdateTime >= interval)
        {
            lastUpdateTime = currentTime;

            for(int i=0;i<4;++i){
                int x = currentTetromino.getBlock()[i].x + currentTetromino.getGlobalPosition().x;
                int y = currentTetromino.getBlock()[i].y + currentTetromino.getGlobalPosition().y;
                if(x >= 0 && x < 10 && y >= 0 && y < 20)
                    boardCells[y][x] = 0;
            }

            if (this->checkCollision(currentTetromino)) {
                this->placePiece(currentTetromino);
                this->clearLines();
                this->spawnTetromino();
            }

            std::lock_guard<std::mutex> lock(currentTetrominoMutex);
            currentTetromino.moveTetrominoDown();

            for(int i=0;i<4;++i){
                int x = currentTetromino.getBlock()[i].x + currentTetromino.getGlobalPosition().x;
                int y = currentTetromino.getBlock()[i].y + currentTetromino.getGlobalPosition().y;
                if(x >= 0 && x < 10 && y >= 0 && y < 20)
                    boardCells[y][x] = currentTetromino.getType() + 1; 
            }

        }
    }
    
}

tetromino ControllerTetris::getCurrentTetromino() {
    return currentTetromino;
}

void ControllerTetris::spawnTetromino(){
    tetromino t;
    int spawnX = BOARD_WIDTH / 2 - 1;
    int spawnY = 0;
    t.setGlobalPosition(spawnX, spawnY);
    this->setCurrentTetromino(t);
};

void ControllerTetris::setCurrentTetromino(const tetromino& t) {
    currentTetromino = t;
}

void ControllerTetris::placePiece(const tetromino& t) {
    const Points* blocks = t.getBlock();
    for(int i=0;i<4;++i){
        int x = blocks[i].x + t.getGlobalPosition().x;
        int y = blocks[i].y + t.getGlobalPosition().y;
        if(x >= 0 && x < BOARD_WIDTH && y >= 0 && y < BOARD_HEIGHT)
            boardCells[y][x] = t.getType() + 1;
    }
    
}

bool ControllerTetris::checkCollision(tetromino& t) {
    const Points* blocks = t.getBlock();
    for(int i=0;i<4;++i){
        int x = blocks[i].x + t.getGlobalPosition().x;
        int y = blocks[i].y + t.getGlobalPosition().y + 1;
        if(x < 0 || x >= BOARD_WIDTH || y < 0 || y >= BOARD_HEIGHT){
            printf("Collision bottom (%d, %d)\n", x, y);
            return true;}
        if(boardCells[y][x] != 0){
            printf("Collision piece (%d, %d)\n", x, y);
            return true;
        }
    }
    return false;
}

void ControllerTetris::clearLines() {
    for(int i = 0; i < BOARD_HEIGHT; ++i) {
        bool lineFull = true;
        for(int j = 0; j < BOARD_WIDTH; ++j) {
            if(boardCells[i][j] == 0) {
                lineFull = false;
                break;
            }
        }

        if(lineFull) {
            for(int k = i; k > 0; --k) {
                for(int j = 0; j < BOARD_WIDTH; ++j) {
                    boardCells[k][j] = boardCells[k-1][j];
                }
            }
            for(int j = 0; j < BOARD_WIDTH; ++j) {
                boardCells[0][j] = 0;
            }
        }
    }
}

void ControllerTetris::moveLeft() {
    // lock the mutex to ensure thread safety
    std::lock_guard<std::mutex> lock(currentTetrominoMutex);
    currentTetromino.moveTetromino(-1, 0);
}

void ControllerTetris::moveRight() {
    std::lock_guard<std::mutex> lock(currentTetrominoMutex);
    currentTetromino.moveTetromino(1, 0);
}

void ControllerTetris::rotate() {
}

void ControllerTetris::softDrop() {
}