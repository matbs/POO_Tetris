#include "Renderer.h"

const Color colors[] = { LIGHTGRAY, RED, GREEN, BLUE, YELLOW };


Renderer::~Renderer() {
    CloseWindow();
}

void Renderer::Initialize() {
    InitWindow(screenWidth, screenHeight, "Tetris");
}

void Renderer::Present() {
    switch (state)
    {
    case State::MENU:
        BeginDrawing();

        ClearBackground(DARKGRAY);
        DrawText("TETRIS", screenWidth / 2-150, screenHeight / 4, 80, LIGHTGRAY);

        EndDrawing();
        break;

    case State::RUNNING:
        BeginDrawing();

        ClearBackground(DARKGRAY);
        for (int i = 0; i < 20; i++) {
            for (int j = 0; j < 10; j++) {
                DrawRectangle(j * 30 + 100, i * 30 + 50, 28, 28, colors[mockBoard[i][j]]);
            }
        }

        EndDrawing();
        break;
    
    default:
        break;
    }
    
}