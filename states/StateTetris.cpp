#include "StateTetris.h"

void StateTetris::Enter() {
    
}

std::unique_ptr<IState> StateTetris::Update() {

    // Check for key inputs
    if( IsKeyDown(KEY_LEFT)) {
        // Move piece left
    } else if( IsKeyDown(KEY_RIGHT)) {
        // Move piece right
    } else if( IsKeyDown(KEY_UP)) {
        // Rotate piece
    } else if( IsKeyDown(KEY_DOWN)) {
        // Soft drop piece
    }

    controller.Draw();
    
    return nullptr;
}

void StateTetris::Exit() {
    
}