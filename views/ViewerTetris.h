#pragma once
#include "../raylib.h"
#include "../controllers/ControllerTetris.h"
#include <vector>
#include <string>
#include <cstdio>

class ViewerTetris {
private:
    ControllerTetris* controller;
    int boardX;
    int boardY; 
    int blockSize;
    
    std::vector<Color> colors = { LIGHTGRAY, RED, GREEN, BLUE, YELLOW, PURPLE, ORANGE, PINK};

    void DrawBlock(int x, int y, int size, Color baseColor, int offsetX, int offsetY);
    void DrawHeart(int x, int y, int size, Color color);
    void DrawDisplay(int posX, int posY, int sizeX, int sizeY);
    
    void DrawBoard();
    void DrawScore();
    void DrawNextPiece();
    void DrawStats();
    void DrawLives();

public:
    ViewerTetris(ControllerTetris* controller, int x, int y, int blockSize = 15);
    
    void Draw();
};