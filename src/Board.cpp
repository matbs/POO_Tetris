#include "../include/Board.h"
#include <iostream>

Board::Board(){
    for(int i=0;i<BOARD_HEIGHT;++i)
        for(int j=0;j<BOARD_WIDTH;++j)
            grid[i][j]=0;
}

Board::~Board(){}

int Board::getCell(int row, int col) const {
    return grid[row][col];
}

void Board::spawnTetromino(){ // Spawns a new tetromino at the top of the board
    tetromino t;
    int spawnX = BOARD_WIDTH / 2 - 1;
    int spawnY = 0;
    t.setGlobalPosition(spawnX, spawnY);  // Set initial position
    setCurrentTetromino(t);
};

bool Board::checkCollision(const tetromino& t) const {
    const Points* blocks = t.getBlock();    // Get the blocks of the tetromino
    for(int i=0;i<4;++i){
        int x = blocks[i].x + t.getGlobalPosition().x; // Calculate global x position in the next right cell
        int y = blocks[i].y + t.getGlobalPosition().y + 1; // Calculate global y position. For some reason y needs +1 to check downward collision
        if(x < 0 || x >= BOARD_WIDTH || y < 0 || y >= BOARD_HEIGHT) // Check bounds
            return true;
        if(grid[y][x] != 0) // Check if the cell is already occupied
            return true;
    }
    return false;
}

tetromino Board::getCurrentTetromino() {
    return currentTetromino;
}

void Board::setCurrentTetromino(const tetromino& t) {
    currentTetromino = t;
}

void Board::placePiece(const tetromino& t) {
    const Points* blocks = t.getBlock();
    for(int i=0;i<4;++i){
        int x = blocks[i].x + t.getGlobalPosition().x;
        int y = blocks[i].y + t.getGlobalPosition().y;
        if(x >= 0 && x < BOARD_WIDTH && y >= 0 && y < BOARD_HEIGHT)
            grid[y][x] = 1; // 1 represents a fixed block on the board
    }
}

void Board::clearLines() {
    for(int i = 0; i < BOARD_HEIGHT; ++i) {
        bool lineFull = true;
        for(int j = 0; j < BOARD_WIDTH; ++j) {
            if(grid[i][j] == 0) {
                lineFull = false;
                break;
            }
        }
        if(lineFull) {
            // Move todas as linhas acima para baixo
            for(int k = i; k > 0; --k) {
                for(int j = 0; j < BOARD_WIDTH; ++j) {
                    grid[k][j] = grid[k-1][j];
                }
            }
            // Limpa a linha superior
            for(int j = 0; j < BOARD_WIDTH; ++j) {
                grid[0][j] = 0;
            }
        }
    }
}

bool Board::checkGameOverSpawn(const tetromino& t) const {
    const Points* blocks = t.getBlock();
    for (int i = 0; i < 4; ++i) {
        int x = blocks[i].x + t.getGlobalPosition().x;
        int y = blocks[i].y + t.getGlobalPosition().y;
        if (x < 0 || x >= BOARD_WIDTH || y < 0 || y >= BOARD_HEIGHT || grid[y][x] != 0) {
            return true;
        }
    }
    return false;
}
