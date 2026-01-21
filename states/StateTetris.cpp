#include "StateTetris.h"

#include <stdio.h>

void StateTetris::Enter() {
    // Initialize thread for controller Tetris
    // controllerThread = std::thread(&ControllerTetris::GameLoop, &controllerTetris);
}

void DrawBlock( int x, int y, int size, Color baseColor, int offsetX = 50, int offsetY = 80) {
    const int BORDER_SIZE = 2;
    float factor = 0.3f;

    Color lightColor = {(unsigned char)(baseColor.r + (255 - baseColor.r) * factor),(unsigned char)(baseColor.g + (255 - baseColor.g) * factor),(unsigned char)(baseColor.b + (255 - baseColor.b) * factor),baseColor.a};
    Color darkColor = {(unsigned char)(baseColor.r * factor), (unsigned char)(baseColor.g * factor), (unsigned char)(baseColor.b * factor), baseColor.a};
    
    int px = x * size + offsetX;
    int py = y * size + offsetY;
    
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

void DrawDisplay(int posX, int posY, int sizeX, int sizeY) {
    DrawRectangle(posX, posY, sizeX, sizeY, BLACK);
    DrawRectangleLines(posX, posY, sizeX, sizeY, MAGENTA);
    DrawRectangleLines(posX - 2, posY - 2, sizeX + 4, sizeY + 4, LIGHTGRAY);
    DrawPixel(posX - 2, posY - 2, DARKGRAY);
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
    DrawDisplay(50, 27, BOARD_WIDTH * 15, 50);
    DrawText("Score:", 55, 30, 20, LIGHTGRAY);
    {
        std::lock_guard<std::mutex> lock(scoreMutex);
        score = controllerTetris.getScore();
    }
    char scoreText[20];
    sprintf(scoreText, "%d", score);
    DrawText(scoreText, 55, 50, 20, LIGHTGRAY);
    
    // Line Display
    DrawDisplay(50, 80 + BOARD_HEIGHT * 15 + 10, (BOARD_WIDTH * 15)/2, 50);
    DrawText("Lines:", 55, 82 + BOARD_HEIGHT * 15 + 10, 20, LIGHTGRAY);
    char linesText[20];
    sprintf(linesText, "%d", controllerTetris.getLinesCleared());
    DrawText(linesText, 55, 105 + BOARD_HEIGHT * 15 + 10, 20, LIGHTGRAY);

    // Next Piece Display
    DrawDisplay(52 + (BOARD_WIDTH * 15)/2 + 2, 80 + BOARD_HEIGHT * 15 + 10, (BOARD_WIDTH * 15)/2, 50);
    // Draw next tetromino blocks here (not implemented)
    tetromino next = controllerTetris.getNextTetromino();
    printf("Next Tetromino Type: %d\n", next.getType());
    const Points* blocks = next.getBlock();
    int offsetX = 70 + (BOARD_WIDTH * 15)/2 + 2;
    int offsetY = 90 + BOARD_HEIGHT * 15 + 10;
    if (next.getType() == 0 || next.getType() == 1) {
        offsetY = 75 + BOARD_HEIGHT * 15 + 10;
    }
    for (int i = 0; i < 4; i++) {
        int x = blocks[i].x; 
        int y = blocks[i].y;
        DrawBlock(x, y, 15, colors[next.getType() + 1], offsetX, offsetY);
    }
    

    // Draw board
    DrawDisplay(50, 80, BOARD_WIDTH * 15, BOARD_HEIGHT * 15);
    
    for (int i = 0; i < 20; i++) {
        for (int j = 0; j < 10; j++) {
            int tileValue = controllerTetris.getCell(i, j);
            if (tileValue != 0) {
                DrawBlock(j, i, 15, colors[tileValue]);
            }
        }
    }

    // Name and Lifes display
    DrawDisplay(50, BOARD_HEIGHT * 15 + 170, BOARD_WIDTH * 15 + 2, 50);
    DrawText("Player", 55, BOARD_HEIGHT * 15 + 170, 20, LIGHTGRAY);
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