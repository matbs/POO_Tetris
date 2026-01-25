#include "StateTetris.h"
#include "StateMenu.h"

#include <stdio.h>

void StateTetris::Enter() {
    viewer = new ViewerTetris(&controllerTetris, 50, 150, 15);
}

std::unique_ptr<IState> StateTetris::Update() {

    controllerTetris.GameLoop();

    Rectangle btnMenuRect = { 50, 20, 100, 40 };
    Vector2 mousePos = GetMousePosition();
    
    bool isMouseOver = CheckCollisionPointRec(mousePos, btnMenuRect);
    
    if (isMouseOver && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        return std::make_unique<StateMenu>(); 
    }

    if( IsKeyDown(KEY_LEFT)) {
        controllerTetris.moveLeft();
    } else if( IsKeyDown(KEY_RIGHT)) {
        controllerTetris.moveRight();
    } else if( IsKeyDown(KEY_UP)) {
        controllerTetris.rotate();
    } else if( IsKeyDown(KEY_DOWN)) {
        controllerTetris.moveDown();
    } else if( IsKeyPressed(KEY_SPACE)) {
        controllerTetris.hardDown();
    } else if (IsKeyPressed(KEY_R)) {
        controllerTetris.resetGame();
    }

    BeginDrawing();

    ClearBackground(DARKGRAY);

    viewer->Draw();

    Color btnColor = isMouseOver ? LIGHTGRAY : GRAY;
    Color textColor = isMouseOver ? BLACK : WHITE;

    DrawRectangleRec(btnMenuRect, btnColor);
    DrawRectangleLinesEx(btnMenuRect, 2, WHITE);

    int textWidth = MeasureText("MENU", 20);
    int textX = btnMenuRect.x + (btnMenuRect.width - textWidth) / 2;
    int textY = btnMenuRect.y + (btnMenuRect.height - 20) / 2;
    DrawText("MENU", textX, textY, 20, textColor);
    
    if (controllerTetris.isGameOver()) {
        DrawText("GAME OVER", 160, 200, 40, RED);
        DrawText("Press [R] to Restart", 130, 250, 20, WHITE);
    }
    
    EndDrawing();
    
    return nullptr;
}

void StateTetris::Exit() {
    if(viewer) delete viewer;
}