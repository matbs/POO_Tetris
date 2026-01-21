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

    while (this->running)
    {
        double currentTime = GetTime();
        if (currentTime - dropTimer >= dropInterval) 
        {
            dropTimer = currentTime;

            this->moveDown();
        }
    }
    
}

tetromino ControllerTetris::getCurrentTetromino() {
    return currentTetromino;
}

void ControllerTetris::hideCurrentTetromino() {
    for(int i=0;i<4;++i){
        int x = currentTetromino.getBlock()[i].x + currentTetromino.getGlobalPosition().x;
        int y = currentTetromino.getBlock()[i].y + currentTetromino.getGlobalPosition().y;
        if(x >= 0 && x < 10 && y >= 0 && y < 20)
            boardCells[y][x] = 0;
    }
}

void ControllerTetris::showCurrentTetromino() {
    for(int i=0;i<4;++i){
        int x = currentTetromino.getBlock()[i].x + currentTetromino.getGlobalPosition().x;
        int y = currentTetromino.getBlock()[i].y + currentTetromino.getGlobalPosition().y;
        if(x >= 0 && x < 10 && y >= 0 && y < 20)
            boardCells[y][x] = currentTetromino.getType() + 1; 
    }
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

bool ControllerTetris::checkCollision(tetromino& t, int dy) {
    const Points* blocks = t.getBlock();
    Points pos = t.getGlobalPosition();
    
    for(int i = 0; i < 4; ++i) {
        int x = blocks[i].x + pos.x;
        int y = blocks[i].y + pos.y + dy;
        
        if(x < 0 || x >= BOARD_WIDTH || y >= BOARD_HEIGHT) {
            return true;
        }
        
        if(y >= 0 && boardCells[y][x] != 0) {
            return true;
        }
    }
    return false;
}

bool ControllerTetris::checkCollisionLateral(tetromino& t, int dx) {
    const Points* blocks = t.getBlock();
    Points pos = t.getGlobalPosition();
    
    for(int i = 0; i < 4; ++i) {
        int x = blocks[i].x + pos.x + dx;
        int y = blocks[i].y + pos.y;
        
        if(x < 0 || x >= BOARD_WIDTH || y >= BOARD_HEIGHT) {
            return true;
        }
        
        if(y >= 0 && boardCells[y][x] != 0) {
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
    double currentTime = GetTime();
    if(currentTime - movementTimer < movementInterval) {
        return; 
    }
    movementTimer = currentTime;

    std::lock_guard<std::mutex> lock(currentTetrominoMutex);
    this->hideCurrentTetromino();

    if (this->checkCollisionLateral(currentTetromino, -1)) {
        this->showCurrentTetromino();
        return;
    }

    currentTetromino.moveTetromino(-1, 0);

    this->showCurrentTetromino();
}

void ControllerTetris::moveRight() {
    double currentTime = GetTime();
    if(currentTime - movementTimer < movementInterval) {
        return; 
    }
    movementTimer = currentTime;

    std::lock_guard<std::mutex> lock(currentTetrominoMutex);
    this->hideCurrentTetromino();

    if (this->checkCollisionLateral(currentTetromino, 1)) {
        this->showCurrentTetromino();
        return;
    }

    currentTetromino.moveTetromino(1, 0);

    this->showCurrentTetromino();
}

void ControllerTetris::moveDown() {
    double currentTime = GetTime();
    if(currentTime - movementTimer < movementInterval) {
        return; 
    }
    movementTimer = currentTime;

    std::lock_guard<std::mutex> lock(currentTetrominoMutex);

    this->hideCurrentTetromino();

    if (this->checkCollision(currentTetromino, 1)) {
        this->placePiece(currentTetromino);
        this->clearLines();
        this->spawnTetromino();
    }

    currentTetromino.moveTetrominoDown();

    this->showCurrentTetromino();
}

void ControllerTetris::rotate() {
    double currentTime = GetTime();
    if(currentTime - movementTimer < movementInterval) {
        return; 
    }
    movementTimer = currentTime;

    std::lock_guard<std::mutex> lock(currentTetrominoMutex);
    this->hideCurrentTetromino();
    
    int oldState = currentTetromino.getRotationState();
    int newState = (oldState + 1) % 4;

    tetromino backup = currentTetromino;

    currentTetromino.rotateTetrominoCW();

    const auto& kicks = tetromino::getSRSKicks(currentTetromino.getType(), oldState, newState);
    
    for (const auto& k : kicks) {

        currentTetromino.moveTetromino(k.x, k.y);
        if (!checkCollision(currentTetromino, 0) && !checkCollisionLateral(currentTetromino, 0)) {
            this->showCurrentTetromino();
            return;
        }
        currentTetromino.moveTetromino(-k.x, -k.y);
    }
    
    currentTetromino = backup;
    this->showCurrentTetromino();
}