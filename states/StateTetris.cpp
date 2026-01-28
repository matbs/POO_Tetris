#include "StateTetris.h"
#include "StateMenu.h"

#include <stdio.h>

void StateTetris::Enter() {
    viewer = new ViewerTetris(&controllerTetris, 200, 80, 25);
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

    ClearBackground(BLACK);

    viewer->Draw();

    DrawButton(btnMenuRect, "MENU", isMouseOver);
    
if (controllerTetris.isGameOver()) {
    std::string winnerText;
    Color textColor;
    Color highlightColor;
    
    DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), {0, 0, 0, 220});

    int panelWidth = 450;
    int panelHeight = 350;
    int panelX = (GetScreenWidth() - panelWidth) / 2;
    int panelY = (GetScreenHeight() - panelHeight) / 2;

    DrawRectangleLines(panelX, panelY, panelWidth, panelHeight, LIGHTGRAY);
    DrawRectangleLines(panelX + 2, panelY + 2, panelWidth - 4, panelHeight - 4, DARKGRAY);

    int titleWidth = MeasureText("GAME OVER", 32);
    DrawText("GAME OVER", panelX + (panelWidth - titleWidth) / 2, panelY + 20, 32, MAGENTA);

    DrawRectangle(panelX + 30, panelY + 65, panelWidth - 60, 1, {100, 100, 100, 150});

    int scoresY = panelY + 90;

    int resultY = panelY + 150;
    int winnerWidth = MeasureText(winnerText.c_str(), 28);

    DrawRectangle(panelX + 50, resultY - 10, panelWidth - 100, 1, {80, 80, 80, 100});

    DrawText(winnerText.c_str(), 
             panelX + (panelWidth - winnerWidth) / 2, 
             resultY, 
             28, 
             textColor);

    DrawRectangle(panelX + 50, resultY + 40, panelWidth - 100, 1, {80, 80, 80, 100});

    int statsY = resultY + 60;
    int linesCleared = controllerTetris.getLinesCleared();

    int statSpacing = 150;
    const char* scoreHeader = "YOUR FINAL SCORE";
    int headerWidth = MeasureText(scoreHeader, 20);
    DrawText(scoreHeader, panelX + (panelWidth - headerWidth) / 2, statsY - 100, 20, {200, 200, 200, 200});

    int scoreWidth = MeasureText(TextFormat("Score: %d", controllerTetris.getScore()), 20);
    DrawText(TextFormat("Score: %d", controllerTetris.getScore()), panelX + (panelWidth - scoreWidth) / 2, statsY - 50, 20, {200, 200, 200, 200});

    int linesWidth = MeasureText(TextFormat("Lines: %d", linesCleared), 20);
    DrawText(TextFormat("Lines: %d", linesCleared), panelX + (panelWidth - linesWidth) / 2, statsY, 20, {200, 200, 200, 200});

    int instructionsY = panelY + panelHeight - 50;

    DrawRectangle(panelX + 40, instructionsY - 20, panelWidth - 80, 1, {60, 60, 60, 100});

    int restartWidth = MeasureText("R - Restart", 20);
    int menuWidth = MeasureText("ESC - Exit Game", 20);
    int totalWidth = restartWidth + 30 + menuWidth;
    
    DrawText("R - Restart", 
             panelX + (panelWidth - totalWidth) / 2, 
             instructionsY, 
             20, 
             GREEN);

    DrawText("ESC - Exit Game", 
             panelX + (panelWidth - totalWidth) / 2 + restartWidth + 30, 
             instructionsY, 
             20, 
             LIGHTGRAY);
}
    
    EndDrawing();
    
    return nullptr;
}

void StateTetris::Exit() {
    if(viewer) delete viewer;
}