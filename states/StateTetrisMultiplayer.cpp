#include "StateTetrisMultiplayer.h"

#include <stdio.h>

void StateTetrisMultiplayer::Enter() {
    viewer2 = new ViewerTetris(&controllerTetris2, 50, 80, 15);
    viewer1 = new ViewerTetris(&controllerTetris1, 300, 80, 15);
}

std::unique_ptr<IState> StateTetrisMultiplayer::Update() {

    controllerTetris1.GameLoop();
    controllerTetris2.GameLoop();

    if( IsKeyDown(KEY_LEFT)) {
        controllerTetris1.moveLeft();
    } else if( IsKeyDown(KEY_RIGHT)) {
        controllerTetris1.moveRight();
    } else if( IsKeyDown(KEY_UP)) {
        controllerTetris1.rotate();
    } else if( IsKeyDown(KEY_DOWN)) {
        controllerTetris1.moveDown();
    }

    if( IsKeyDown(KEY_A)) {
        controllerTetris2.moveLeft();
    } else if( IsKeyDown(KEY_D)) {
        controllerTetris2.moveRight();
    } else if( IsKeyDown(KEY_W)) {
        controllerTetris2.rotate();
    } else if( IsKeyDown(KEY_S)) {
        controllerTetris2.moveDown();
    }

    BeginDrawing();

    ClearBackground(DARKGRAY);

    viewer1->Draw();
    viewer2->Draw();
    
    EndDrawing();
    
    return nullptr;
}

void StateTetrisMultiplayer::Exit() {
    if(viewer1) delete viewer1;
    if(viewer2) delete viewer2;
}