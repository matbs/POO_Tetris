#include "StateTetrisMultiplayer.h"

#include <stdio.h>

void StateTetrisMultiplayer::Enter() {
    viewer2 = new ViewerTetris(&controllerTetris2, 50, 80, 15);
    viewer1 = new ViewerTetris(&controllerTetris1, 300, 80, 15);
}

std::unique_ptr<IState> StateTetrisMultiplayer::Update() {

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
        
        switch (winner) {
            case 1:
                winnerText = "PLAYER 1 WON!";
                textColor = RED;
                break;
            case 2:
                winnerText = "PLAYER 2 WON!";
                textColor = BLUE;
                break;
            case 3:
                winnerText = "TIE!";
                textColor = YELLOW;
                break;
        }
        
        int score1 = controllerTetris1.getScore();
        int score2 = controllerTetris2.getScore();
        DrawRectangle(140, 10, 300, 210, BLACK);
        DrawText("FINAL SCORE", 150, 20, 30, WHITE);
        DrawText(TextFormat("Player 1: %d", score1), 150, 60, 25, RED);
        DrawText(TextFormat("Player 2: %d", score2), 150, 90, 25, BLUE);
        DrawText(winnerText.c_str(), 150, 130, 35, textColor);
        DrawText("Press R to reset the game", 150, 180, 20, GREEN);
    }

    EndDrawing();
    
    return nullptr;
}

void StateTetrisMultiplayer::Exit() {
    if(viewer1) delete viewer1;
    if(viewer2) delete viewer2;
}