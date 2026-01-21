#pragma once
#include "raylib.h"
#include "../models/Tetrominos.h"
#include <thread>
#include <chrono>
#include <mutex>

#define BOARD_WIDTH 10
#define BOARD_HEIGHT 20

const Color colors[] = { LIGHTGRAY, RED, GREEN, BLUE, YELLOW, PURPLE, ORANGE, PINK};

class ControllerTetris
{
private:
    bool running = true;
    int boardCells[20][10] = {{0}};

public:
    ControllerTetris();
    ~ControllerTetris();

    void GameLoop();
    void Stop() { running = false; }
    int getCell(int row, int col) const { return boardCells[row][col]; }

    tetromino getCurrentTetromino();
    void setCurrentTetromino(const tetromino& t);
    void setNextTetromino(const tetromino& t);
    tetromino getNextTetromino();
    void hideCurrentTetromino();
    void showCurrentTetromino();

    void spawnTetromino(tetromino& t); 
    bool checkCollision( tetromino& t, int dy); 
    bool checkCollisionLateral( tetromino& t, int dx);
    void placePiece(const tetromino& t);
    void clearLines();
    bool checkGameOverSpawn(const tetromino& t) const;

    void moveLeft();
    void moveRight();
    void moveDown();
    void rotate();

    bool checkTimer(float& timer, float interval);

    tetromino currentTetromino; 
    tetromino nextTetromino;

    std::mutex currentTetrominoMutex;

    void setScore(int newScore) { score = newScore; }
    int getScore() { return score; }

    void setLinesCleared(int newLines) { linesCleared = newLines; }
    int getLinesCleared() { return linesCleared; }

    private:
        float movementTimer = 0.0f;
        float movementInterval = 0.1f;
        
        float dropTimer = 0.0f;
        float dropInterval = 1.0f;

        int score = 0;
        int linesCleared = 0;
        int level = 0;
};
