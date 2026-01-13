#include "StateTetris.h"

#include <stdio.h>

void StateTetris::Enter() {
    // Initialize thread for controller Tetris
    controllerThread = std::thread(&ControllerTetris::GameLoop, &controllerTetris);

}

void DrawBlock(int x, int y, int size, Color baseColor) {
    const int BORDER_SIZE = 2;
    float factor = 0.3f;

    Color lightColor = {(unsigned char)(baseColor.r + (255 - baseColor.r) * factor),(unsigned char)(baseColor.g + (255 - baseColor.g) * factor),(unsigned char)(baseColor.b + (255 - baseColor.b) * factor),baseColor.a};
    Color darkColor = {(unsigned char)(baseColor.r * factor), (unsigned char)(baseColor.g * factor), (unsigned char)(baseColor.b * factor), baseColor.a};
    
    int px = x * size + 50;
    int py = y * size + 25;
    
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

std::unique_ptr<IState> StateTetris::Update() {

    // Check for key inputs
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
        controllerTetris.softDrop();
    }

    BeginDrawing();

    ClearBackground(DARKGRAY);
    DrawRectangle(50, 25, 150, 300, BLACK); 
    DrawRectangleLines(49, 24, 152, 302, MAGENTA);
    DrawRectangleLines(47, 22, 156, 306, LIGHTGRAY);
    DrawPixel(47, 22, DARKGRAY);
    
    for (int i = 0; i < 20; i++) {
        for (int j = 0; j < 10; j++) {
            int tileValue = controllerTetris.getCell(i, j);
            if (tileValue != 0) {
                //printf("Tetromino block at (%d, %d) with value %d\n", j, i, tileValue); // Debug print
                DrawBlock(j, i, 15, colors[tileValue]);
            }
        }
    }
    

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