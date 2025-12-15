#pragma once
#include "raylib.h"
#include "../include/Board.h"
#include <thread>
#include <chrono>

const Color colors[] = { LIGHTGRAY, RED, GREEN, BLUE, YELLOW };

class ControllerTetris
{
private:
    Board board;
    int boardCells[20][10] = {{0}};

public:
    ControllerTetris();
    ~ControllerTetris();

    void GameLoop();
    int getCell(int row, int col) const { return boardCells[row][col]; }
};
