#pragma once
#include "../raylib.h"
#include <memory>
#include <string>

// Interface for game states
class IState {
public:
    virtual void Enter() = 0;
    virtual std::unique_ptr<IState> Update() = 0;
    virtual void Exit() = 0;

    void DrawButton(Rectangle rect, const char* text, bool isMouseOver);
};