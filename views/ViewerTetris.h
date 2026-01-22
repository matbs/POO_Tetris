#pragma once
#include "raylib.h"
#include "../controllers/ControllerTetris.h"
#include <vector>
#include <string>
#include <cstdio>

class ViewerTetris {
private:
    ControllerTetris* controller;
    int boardX; // Posição X do canto superior esquerdo do tabuleiro
    int boardY; // Posição Y do canto superior esquerdo do tabuleiro
    int blockSize;
    
    // Cores padrão do Tetris
    std::vector<Color> colors = { LIGHTGRAY, RED, GREEN, BLUE, YELLOW, PURPLE, ORANGE, PINK};

    // Métodos auxiliares de desenho internos
    void DrawBlock(int x, int y, int size, Color baseColor, int offsetX, int offsetY);
    void DrawHeart(int x, int y, int size, Color color);
    void DrawDisplay(int posX, int posY, int sizeX, int sizeY);
    
    // Partes da UI
    void DrawBoard();
    void DrawScore();
    void DrawNextPiece();
    void DrawStats();
    void DrawLives();

public:
    // Construtor recebe o controlador e a posição onde o TABULEIRO será desenhado
    ViewerTetris(ControllerTetris* controller, int x, int y, int blockSize = 15);
    
    // Método principal chamado no GameLoop
    void Draw();
};