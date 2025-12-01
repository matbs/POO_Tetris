#include "StateMenu.h"
#include "StateTetris.h"
#include <cstdio>
#include <memory>

void StateMenu::Enter() {}

std::unique_ptr<IState> StateMenu::Update() {
    BeginDrawing();

    ClearBackground(DARKGRAY);
    DrawText("TETRIS", screenWidth / 2-150, screenHeight / 4, 80, LIGHTGRAY);

    // Draw a square as a placeholder for the start button
    DrawRectangleRounded({(float)(screenWidth / 2 - 100), (float)(screenHeight / 2 - 25), 200, 50}, 0.2f, 10, GRAY);
    DrawText("START", screenWidth / 2 - 50, screenHeight / 2 - 10, 30, LIGHTGRAY);

    // check for mouse click on the start button
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        Vector2 mousePos = GetMousePosition();
        if (mousePos.x >= screenWidth / 2 - 100 && mousePos.x <= screenWidth / 2 + 100 &&
            mousePos.y >= screenHeight / 2 - 25 && mousePos.y <= screenHeight / 2 + 25) {
            // Transition to the Tetris state
            return std::make_unique<StateTetris>();
        }
    }

    EndDrawing();
    return nullptr;
}

void StateMenu::Exit() {}