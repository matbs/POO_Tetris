#include "./raylib.h"
#include "./states/IState.h"
#include "./states/StateMenu.h"

int main(){
    const int screenWidth = 600;
    const int screenHeight = 800;
    InitWindow(screenWidth, screenHeight, "Tetris");

    StateMenu menuState;
    std::unique_ptr<IState> currentState = std::make_unique<StateMenu>(menuState);
    std::unique_ptr<IState> nextState = nullptr;
    currentState->Enter();

    SetTargetFPS(60);
    while (WindowShouldClose() == false)
    {
        nextState = currentState->Update();
        if (nextState) {
            currentState->Exit();
            currentState = std::move(nextState);
            currentState->Enter();
        }
    }
    currentState->Exit();

    CloseWindow();

    return 0;
}