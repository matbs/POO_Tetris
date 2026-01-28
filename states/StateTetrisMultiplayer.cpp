#include "StateTetrisMultiplayer.h"
#include "StateMenu.h"

#include <stdio.h>

void StateTetrisMultiplayer::DrawEndScreen(int p1Score, int p2Score, int winner, std::string resultText, Color resultColor, std::string play1 = "PLAYER 1", std::string play2 = "PLAYER 2") {
        DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), {0, 0, 0, 220});

        int panelWidth = 500;
        int panelHeight = 400;
        int panelX = (GetScreenWidth() - panelWidth) / 2;
        int panelY = (GetScreenHeight() - panelHeight) / 2;

        DrawRectangleLines(panelX, panelY, panelWidth, panelHeight, LIGHTGRAY);
        DrawRectangleLines(panelX + 2, panelY + 2, panelWidth - 4, panelHeight - 4, DARKGRAY);

        int titleWidth = MeasureText("GAME OVER", 32);
        DrawText("GAME OVER", panelX + (panelWidth - titleWidth) / 2, panelY + 20, 32, MAGENTA);

        DrawRectangle(panelX + 30, panelY + 65, panelWidth - 60, 1, {100, 100, 100, 150});

        int resultWidth = MeasureText(resultText.c_str(), 36);
        DrawText(resultText.c_str(), panelX + (panelWidth - resultWidth) / 2, panelY + 90, 36, resultColor);

        DrawRectangle(panelX + 50, panelY + 170, panelWidth - 100, 1, {80, 80, 80, 100});

        int statsY = panelY + 190;

        DrawText("SCOREBOARD", panelX + (panelWidth - MeasureText("SCOREBOARD", 24)) / 2, statsY, 24, LIGHTGRAY);

        int scoresY = statsY + 40;

        Color p1ScoreColor = p1Score > p2Score ? GREEN : (winner == 0 ? YELLOW : LIGHTGRAY);
        Color p2ScoreColor = p2Score > p1Score ? GREEN : (winner == 0 ? YELLOW : LIGHTGRAY);

        int scorePanelWidth = 200;
        int leftScoreX = panelX + (panelWidth/2 - scorePanelWidth - 20);
        int rightScoreX = panelX + (panelWidth/2 + 20);

        DrawText(play1.c_str(), leftScoreX, scoresY, 18, LIGHTGRAY);
        DrawText(TextFormat("%d", p1Score), 
                leftScoreX + (scorePanelWidth - MeasureText(TextFormat("%d", p1Score), 28)) / 2, 
                scoresY + 30, 28, p1ScoreColor);

        DrawText(play2.c_str(), rightScoreX, scoresY, 18, LIGHTGRAY);
        DrawText(TextFormat("%d", p2Score), 
                rightScoreX + (scorePanelWidth - MeasureText(TextFormat("%d", p2Score), 28)) / 2, 
                scoresY + 30, 28, p2ScoreColor);

        DrawRectangle(panelX + panelWidth/2 - 1, scoresY - 10, 2, 80, {100, 100, 100, 150});

        DrawRectangle(panelX + 40, panelY + panelHeight - 70, panelWidth - 80, 1, {60, 60, 60, 100});
}

void StateTetrisMultiplayer::Enter() {
    viewer2 = new ViewerTetris(&controllerTetris2, 50, 150, 20);
    viewer1 = new ViewerTetris(&controllerTetris1, 300, 150, 20);

    tetromino ghostPiece;
    ghostPiece.setGlobalPosition(-100, -100);
    
    controllerTetris1.setCurrentTetromino(ghostPiece);
}

std::unique_ptr<IState> StateTetrisMultiplayer::Update() {

    Rectangle btnMenuRect = { 50, 20, 100, 40 };
    Vector2 mousePos = GetMousePosition();
    
    bool isMouseOver = CheckCollisionPointRec(mousePos, btnMenuRect);
    
    if (isMouseOver && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        return std::make_unique<StateMenu>(); 
    }

    if(currentPhase == MultiplayerPhase::PLAYING) {

        controllerTetris1.GameLoop();
        controllerTetris2.GameLoop();

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

        ClearBackground(BLACK);

        viewer1->Draw();
        viewer2->Draw();

        DrawButton(btnMenuRect, "MENU", isMouseOver);

        EndDrawing();

        if (controllerTetris1.isGameOver() && controllerTetris2.isGameOver()) {
            currentPhase = MultiplayerPhase::GAME_OVER;
        }
    }
    else if (currentPhase == MultiplayerPhase::GAME_OVER) {

        if (controllerTetris1.getScore() > controllerTetris2.getScore()) {
            winner = 2;
        } else if (controllerTetris2.getScore() > controllerTetris1.getScore()) {
            winner = 1;
        } else {
            winner = 0;
        }
        BeginDrawing();
        ClearBackground(BLACK);

        viewer1->Draw();
        viewer2->Draw();

        DrawEndScreen(controllerTetris1.getScore(), controllerTetris2.getScore(), winner,
                      winner == 0 ? "IT'S A TIE!" : (winner == 1 ? "PLAYER 1 WINS!" : "PLAYER 2 WINS!"), YELLOW, "PLAYER 1", "PLAYER 2");
        DrawButton(btnMenuRect, "MENU", isMouseOver);

        EndDrawing();
    }
    
    return nullptr;
}

void StateTetrisMultiplayer::Exit() {
    if(viewer1) delete viewer1;
    if(viewer2) delete viewer2;
}