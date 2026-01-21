#include "StateTetris.h"

#include <stdio.h>

void StateTetris::Enter() {
    // Initialize thread for controller Tetris
    // controllerThread = std::thread(&ControllerTetris::GameLoop, &controllerTetris);
}

void DrawBlock(int x, int y, int size, Color baseColor) {
    const int BORDER_SIZE = 2;
    float factor = 0.3f;

    Color lightColor = {(unsigned char)(baseColor.r + (255 - baseColor.r) * factor),(unsigned char)(baseColor.g + (255 - baseColor.g) * factor),(unsigned char)(baseColor.b + (255 - baseColor.b) * factor),baseColor.a};
    Color darkColor = {(unsigned char)(baseColor.r * factor), (unsigned char)(baseColor.g * factor), (unsigned char)(baseColor.b * factor), baseColor.a};
    
    int px = x * size + 50;
    int py = y * size + 80;
    
    DrawTriangle(
        { (float)(px),          (float)(py + size) },   
        { (float)(px + size),   (float)(py + size) },   
        { (float)(px + size),   (float)(py) },          
        darkColor
    );

    DrawTriangle(
        { (float)(px),          (float)(py) },          
        { (float)(px),          (float)(py + size) },   
        { (float)(px + size),   (float)(py) },          
        lightColor
    );

    DrawRectangle(
        px + BORDER_SIZE, 
        py + BORDER_SIZE, 
        size - 2 * BORDER_SIZE, 
        size - 2 * BORDER_SIZE, 
        baseColor
    );
}

void DrawHeart(int x, int y, int size, Color color) {
    int px = x * size + 50;
    int py = y * size + 80;

    DrawTriangle(
        { (float)(px + size / 2), (float)(py + size) },
        { (float)(px),            (float)(py + size / 2) },
        { (float)(px + size),     (float)(py + size / 2) },
        color
    );
    DrawCircle(px + size / 4, py + size / 4, size / 4, color);
    DrawCircle(px + 3 * size / 4, py + size / 4, size / 4, color);
}

std::unique_ptr<IState> StateTetris::Update() {

    controllerTetris.GameLoop();

    if( IsKeyDown(KEY_LEFT)) {
        printf("Move Left\n");
        controllerTetris.moveLeft();
    } else if( IsKeyDown(KEY_RIGHT)) {
        printf("Move Right\n");
        controllerTetris.moveRight();
    } else if( IsKeyDown(KEY_UP)) {
        printf("Rotate\n");
        controllerTetris.rotate();
    } else if( IsKeyDown(KEY_DOWN)) {
        printf("Soft Drop\n");
        controllerTetris.moveDown();
    }

    BeginDrawing();

    ClearBackground(DARKGRAY);
    

    // Score display
    DrawRectangle(50, 25, BOARD_WIDTH * 15 + 2, BOARD_HEIGHT * 15 + 2, BLACK);
    DrawRectangleLines(49, 24, BOARD_WIDTH * 15 + 2, BOARD_HEIGHT * 15 + 2, MAGENTA);
    DrawRectangleLines(47, 22, BOARD_WIDTH * 15 + 6, BOARD_HEIGHT * 15 + 6, LIGHTGRAY);
    DrawPixel(47, 22, DARKGRAY);
    DrawText("Score:", 55, 27, 20, LIGHTGRAY);
    {
        std::lock_guard<std::mutex> lock(scoreMutex);
        score = controllerTetris.getScore();
    }
    char scoreText[20];
    sprintf(scoreText, "%d", score);
    DrawText(scoreText, 55, 50, 20, LIGHTGRAY);
    
    // Line Display
    DrawRectangle(50, 80 + BOARD_HEIGHT * 15 + 10, (BOARD_WIDTH * 15)/2, 60, BLACK);
    DrawRectangleLines(49, 79 + BOARD_HEIGHT * 15 + 10, (BOARD_WIDTH * 15)/2, 60, MAGENTA);
    DrawRectangleLines(47, 77 + BOARD_HEIGHT * 15 + 10, (BOARD_WIDTH * 15)/2 + 4, 64, LIGHTGRAY);
    DrawPixel(47, 77 + BOARD_HEIGHT * 15 + 10, DARKGRAY);
    DrawText("Lines:", 55, 82 + BOARD_HEIGHT * 15 + 10, 20, LIGHTGRAY);
    char linesText[20];
    sprintf(linesText, "%d", controllerTetris.getLinesCleared());
    DrawText(linesText, 55, 105 + BOARD_HEIGHT * 15 + 10, 20, LIGHTGRAY);

    // Next Piece Display
    DrawRectangle(50 + (BOARD_WIDTH * 15)/2 + 2, 80 + BOARD_HEIGHT * 15 + 10, (BOARD_WIDTH * 15)/2, 50, BLACK);
    DrawRectangleLines(49 + (BOARD_WIDTH * 15)/2 + 2, 79 + BOARD_HEIGHT * 15 + 10, (BOARD_WIDTH * 15)/2, 50, MAGENTA);
    DrawRectangleLines(47 + (BOARD_WIDTH * 15)/2 + 2, 77 + BOARD_HEIGHT * 15 + 10, (BOARD_WIDTH * 15)/2 + 4, 54, LIGHTGRAY);
    DrawPixel(47 + (BOARD_WIDTH * 15)/2 + 2, 77 + BOARD_HEIGHT * 15 + 10, DARKGRAY);
    DrawText("Next:", 55 + (BOARD_WIDTH * 15)/2 + 2, 82 + BOARD_HEIGHT * 15 + 10, 20, LIGHTGRAY);
    // Draw next tetromino blocks here (not implemented)


    // Draw board
    DrawRectangle(50, 80, BOARD_WIDTH * 15, BOARD_HEIGHT * 15, BLACK);
    DrawRectangleLines(49, 79, BOARD_WIDTH * 15 + 2, BOARD_HEIGHT * 15 + 2, MAGENTA);
    DrawRectangleLines(47, 77, BOARD_WIDTH * 15 + 6, BOARD_HEIGHT * 15 + 6, LIGHTGRAY);
    DrawPixel(47, 77, DARKGRAY);
    
    for (int i = 0; i < 20; i++) {
        for (int j = 0; j < 10; j++) {
            int tileValue = controllerTetris.getCell(i, j);
            if (tileValue != 0) {
                DrawBlock(j, i, 15, colors[tileValue]);
            }
        }
    }

    // Name and Lifes display
    DrawRectangle(50, BOARD_HEIGHT * 15 + 140, BOARD_WIDTH * 15 + 2, 50, BLACK);
    DrawRectangleLines(49, BOARD_HEIGHT * 15 + 139, BOARD_WIDTH * 15 + 2, 50, MAGENTA);
    DrawRectangleLines(47, BOARD_HEIGHT * 15 + 137, BOARD_WIDTH * 15 + 6, 54, LIGHTGRAY);
    DrawPixel(47, BOARD_HEIGHT * 15 + 127, DARKGRAY);
    DrawText("Player", 55, BOARD_HEIGHT * 15 + 140, 20, LIGHTGRAY);
    // draw 3 hearts for lives
    for (int i = 0; i < 3; i++) {
        DrawHeart(55 + i * 30, BOARD_HEIGHT * 15 + 160, 20, RED);
    }
    // Options (change speed, pause, quit)
    // EndGame display
    
    EndDrawing();
    
    return nullptr;
}

void StateTetris::Exit() {
    controllerTetris.Stop();
    if (controllerThread.joinable())
    {
        controllerThread.join();
    }
}