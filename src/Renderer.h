#pragma once
#include "raylib.h"

enum class State {
    MENU,
    RUNNING,
    PAUSED,
    GAME_OVER
};

class Renderer {

public:
    Renderer(): screenWidth(600), screenHeight(800), state(State::RUNNING) {}
    Renderer(int screenWidth, int screenHeight): screenWidth(screenWidth), screenHeight(screenHeight), state(State::MENU) {}
    ~Renderer();
    void Initialize();
    void Present();

private:
    int screenWidth;
    int screenHeight;
    State state;

    int mockBoard[20][10] = {{0,0,0,0,0,0,0,0,0,0}, 
                            {0,0,0,0,0,0,0,0,0,0},
                            {0,0,0,0,0,0,0,0,0,0},
                            {0,0,0,0,0,0,0,0,0,0},
                            {0,0,0,0,0,0,0,0,0,0},
                            {0,0,0,0,0,0,0,0,0,0},
                            {0,0,0,0,0,0,0,0,0,0},
                            {0,0,0,0,0,0,0,0,0,0},
                            {0,0,0,0,0,0,0,0,0,0},
                            {0,0,0,0,0,0,0,0,0,0},
                            {0,0,0,0,0,0,1,1,1,1},
                            {0,0,0,0,0,0,1,1,2,2},
                            {0,0,2,2,3,3,3,3,3,1},
                            {0,0,0,0,0,0,0,0,0,0},
                            {0,0,0,0,0,0,0,0,0,0},
                            {0,0,0,0,0,0,0,0,0,0},
                            {0,0,0,0,0,0,0,0,0,0},
                            {0,0,0,0,0,0,0,0,0,0},
                            {0,0,0,0,0,0,0,0,0,0},
                            {0,0,0,0,0,0,0,0,0,0}};
     // Mock board for demonstration
};