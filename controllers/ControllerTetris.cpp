#include "ControllerTetris.h"

ControllerTetris::ControllerTetris() {}

ControllerTetris::~ControllerTetris() {}

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

void ControllerTetris::Draw() {
    BeginDrawing();

    ClearBackground(DARKGRAY);
    DrawRectangle(50, 25, 150, 300, BLACK); 
    DrawRectangleLines(49, 24, 152, 302, MAGENTA);
    DrawRectangleLines(47, 22, 156, 306, LIGHTGRAY);
    DrawPixel(47, 22, DARKGRAY);
    
    for (int i = 0; i < 20; i++) {
        for (int j = 0; j < 10; j++) {
            int tileValue = mockBoard[i][j];
            if (tileValue != 0) {
                DrawBlock(j, i, 15, colors[tileValue]);
            }
        }
    }

    DrawText("SCORE: 0000", 420, 300, 20, LIGHTGRAY);
    DrawText("LEVEL: 01", 420, 330, 20, LIGHTGRAY);

    DrawRectangle(450, 50, 120, 160, BLACK);
    DrawRectangleLines(449, 49, 122, 162, LIGHTGRAY);
    DrawText("NEXT", 470, 220, 20, LIGHTGRAY);


    EndDrawing();
}