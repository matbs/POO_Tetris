#pragma once
#include "raylib.h"
#include <memory>

// Interface for game states
class IState {
public:
    virtual void Enter() = 0;
    virtual std::unique_ptr<IState> Update() = 0;
    virtual void Exit() = 0;
protected:
    int screenWidth = 600;
    int screenHeight = 800;
};