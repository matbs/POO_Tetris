#include "StateTetrisMultiplayer.h"
#include "StateMenu.h"

#include <stdio.h>

void StateTetrisMultiplayer::Enter() {
    viewer2 = new ViewerTetris(&controllerTetris2, 50, 150, 15);
    viewer1 = new ViewerTetris(&controllerTetris1, 300, 150, 15);
}

std::unique_ptr<IState> StateTetrisMultiplayer::Update() {

    Rectangle btnMenuRect = { 50, 20, 100, 40 };
    Vector2 mousePos = GetMousePosition();
    
    bool isMouseOver = CheckCollisionPointRec(mousePos, btnMenuRect);
    
    if (isMouseOver && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        return std::make_unique<StateMenu>(); 
    }

    controllerTetris1.GameLoop();
    controllerTetris2.GameLoop();

    if (winner == 0) {
        if (controllerTetris1.isGameOver() && controllerTetris2.isGameOver()) {
            int score1 = controllerTetris1.getScore();
            int score2 = controllerTetris2.getScore();
            if (score1 > score2) {
                winner = 1;
            } else if (score2 > score1) {
                winner = 2;
            } else {
                winner = 3;
            }
        }
    }

    if (IsKeyPressed(KEY_R))
    {
        controllerTetris1.resetGame();
        controllerTetris2.resetGame();
        winner = 0;
    }
    

    if( IsKeyDown(KEY_LEFT)) {
        controllerTetris1.moveLeft();
    } else if( IsKeyDown(KEY_RIGHT)) {
        controllerTetris1.moveRight();
    } else if( IsKeyDown(KEY_UP)) {
        controllerTetris1.rotate();
    } else if( IsKeyDown(KEY_DOWN)) {
        controllerTetris1.moveDown();
    } else if (IsKeyPressed(KEY_KP_0)) {
        controllerTetris1.hardDown();
    }

    if( IsKeyDown(KEY_A)) {
        controllerTetris2.moveLeft();
    } else if( IsKeyDown(KEY_D)) {
        controllerTetris2.moveRight();
    } else if( IsKeyDown(KEY_W)) {
        controllerTetris2.rotate();
    } else if( IsKeyDown(KEY_S)) {
        controllerTetris2.moveDown();
    } else if( IsKeyPressed(KEY_LEFT_SHIFT)) {
        controllerTetris2.hardDown();
    }

    BeginDrawing();

    ClearBackground(DARKGRAY);

    viewer1->Draw();
    viewer2->Draw();
    
    if (winner > 0) {
        std::string winnerText;
        Color textColor;
        Color highlightColor;
        
        switch (winner) {
            case 1:
                winnerText = "PLAYER 1 WINS";
                textColor = RED;
                highlightColor = RED;
                break;
            case 2:
                winnerText = "PLAYER 2 WINS";
                textColor = BLUE;
                highlightColor = BLUE;
                break;
            case 3:
                winnerText = "DRAW";
                textColor = YELLOW;
                highlightColor = GRAY;
                break;
        }
        
        int score1 = controllerTetris1.getScore();
        int score2 = controllerTetris2.getScore();

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

        DrawText("P1", panelX + 70, scoresY, 24, RED);
        DrawRectangle(panelX + 60, scoresY + 30, 60, 2, RED);
        DrawText(TextFormat("%d", score1), panelX + 65, scoresY + 40, 36, (winner == 1) ? GOLD : WHITE);

        DrawText("VS", panelX + panelWidth/2 - 10, scoresY + 15, 20, LIGHTGRAY);

        DrawText("P2", panelX + panelWidth - 110, scoresY, 24, BLUE);
        DrawRectangle(panelX + panelWidth - 120, scoresY + 30, 60, 2, BLUE);
        DrawText(TextFormat("%d", score2), panelX + panelWidth - 115, scoresY + 40, 36, (winner == 2) ? GOLD : WHITE);

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
        int lines1 = controllerTetris1.getLinesCleared();
        int lines2 = controllerTetris2.getLinesCleared();

        int statSpacing = 150;
        
        DrawText(TextFormat("Lines: %d", lines1), panelX + 60, statsY, 20, {200, 200, 200, 200});
        DrawText(TextFormat("Lines: %d", lines2), panelX + panelWidth - 160, statsY, 20, {200, 200, 200, 200});

        int instructionsY = panelY + panelHeight - 50;

        DrawRectangle(panelX + 40, instructionsY - 20, panelWidth - 80, 1, {60, 60, 60, 100});

        int restartWidth = MeasureText("R - Restart", 20);
        int totalWidth = restartWidth;
        
        DrawText("R - Restart", 
                 panelX + (panelWidth - totalWidth) / 2, 
                 instructionsY, 
                 20, 
                 LIGHTGRAY);
        
        if (winner != 3) {
            int highlightX = (winner == 1) ? panelX + 55 : panelX + panelWidth - 55;
            DrawRectangle(highlightX, scoresY, 2, 80, {textColor.r, textColor.g, textColor.b, 100});
        }
    }


    DrawButton(btnMenuRect, "MENU", isMouseOver);

    EndDrawing();
    
    return nullptr;
}

void StateTetrisMultiplayer::Exit() {
    if(viewer1) delete viewer1;
    if(viewer2) delete viewer2;
}