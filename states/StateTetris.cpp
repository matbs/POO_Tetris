#include "StateTetris.h"

#include <stdio.h>

void StateTetris::Enter() {
    viewer = new ViewerTetris(&controllerTetris, 50, 80, 15);
}

std::unique_ptr<IState> StateTetris::Update() {

    controllerTetris.GameLoop();

    if( IsKeyDown(KEY_LEFT)) {
        printf("Move Left\n");
        controllerTetris.moveLeft();
    } else if( IsKeyDown(KEY_RIGHT)) {
        printf("Move Right\n");
        controllerTetris.moveRight();
    } else if( IsKeyDown(KEY_UP)) {
        printf("Rotate\n");
        controllerTetris.rotate();
    } else if( IsKeyDown(KEY_DOWN)) {
        printf("Soft Drop\n");
        controllerTetris.moveDown();
    } else if( IsKeyPressed(KEY_SPACE)) {
        printf("Hard Drop\n");
        controllerTetris.hardDown();
    }

    BeginDrawing();

    ClearBackground(DARKGRAY);

    viewer->Draw();
    
    EndDrawing();
    
    return nullptr;
}

void StateTetris::Exit() {
    if(viewer) delete viewer;
}