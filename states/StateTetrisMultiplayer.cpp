#include "StateTetrisMultiplayer.h"
#include "StateMenu.h"

#include <stdio.h>

void StateTetrisMultiplayer::Enter() {
    viewer2 = new ViewerTetris(&controllerTetris2, 50, 150, 20);
    viewer1 = new ViewerTetris(&controllerTetris1, 300, 150, 20);
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
        bool localWon = false;
        bool remoteWon = false;
        bool tie = false;

        if (controllerTetris1.getScore() > controllerTetris2.getScore()) {
            localWon = true;
        } else if (controllerTetris2.getScore() > controllerTetris1.getScore()) {
            remoteWon = true;
        } else {
            tie = true;
        }
        BeginDrawing();
        ClearBackground(BLACK);

        viewer1->Draw();
        viewer2->Draw();

        DrawEndScreen(controllerTetris1.getScore(), controllerTetris2.getScore(), localWon, remoteWon, tie);
        DrawButton(btnMenuRect, "MENU", isMouseOver);

        EndDrawing();
    }
    
    return nullptr;
}

void StateTetrisMultiplayer::Exit() {
    if(viewer1) delete viewer1;
    if(viewer2) delete viewer2;
}