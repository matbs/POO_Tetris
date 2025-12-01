#include "StateTetris.h"

void DrawBlock(int x, int y, int size, Color baseColor) {
    const int BORDER_SIZE = 3;
    float factor = 0.3f;

    Color lightColor = {(unsigned char)(baseColor.r + (255 - baseColor.r) * factor),(unsigned char)(baseColor.g + (255 - baseColor.g) * factor),(unsigned char)(baseColor.b + (255 - baseColor.b) * factor),baseColor.a};
    Color darkColor = {(unsigned char)(baseColor.r * factor), (unsigned char)(baseColor.g * factor), (unsigned char)(baseColor.b * factor), baseColor.a};
    
    int px = x * size + 100;
    int py = y * size + 50;
    
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

void StateTetris::Enter() {}

std::unique_ptr<IState> StateTetris::Update() {
    // Main game loop logic for Tetris
    BeginDrawing();

    // Check for key inputs
    if( IsKeyDown(KEY_LEFT)) {
        // Move piece left
    } else if( IsKeyDown(KEY_RIGHT)) {
        // Move piece right
    } else if( IsKeyDown(KEY_UP)) {
        // Rotate piece
    } else if( IsKeyDown(KEY_DOWN)) {
        // Soft drop piece
    }

    ClearBackground(DARKGRAY);
    for (int i = 0; i < 20; i++) {
        for (int j = 0; j < 10; j++) {
            int tileValue = mockBoard[i][j];
            if (tileValue != 0) {
                DrawBlock(j, i, 30, colors[tileValue]);
            } else {
                DrawRectangle(j * 30 + 100, i * 30 + 50, 28, 28, LIGHTGRAY);
            }
        }
    }

    EndDrawing();
    return nullptr;
}

void StateTetris::Exit() {}