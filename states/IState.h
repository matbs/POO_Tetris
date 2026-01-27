#pragma once
#include "raylib.h"
#include <memory>

// Interface for game states
class IState {
public:
    virtual void Enter() = 0;
    virtual std::unique_ptr<IState> Update() = 0;
    virtual void Exit() = 0;
    
    void DrawButton(Rectangle rect, const char* text, bool isMouseOver) {
        Color btnColor = isMouseOver ? LIGHTGRAY : GRAY;
        Color textColor = isMouseOver ? BLACK : WHITE;

        DrawRectangleRec(rect, btnColor);
        DrawRectangleLinesEx(rect, 2, WHITE);

        int textWidth = MeasureText(text, 20);
        int textX = rect.x + (rect.width - textWidth) / 2;
        int textY = rect.y + (rect.height - 20) / 2;
        DrawText(text, textX, textY, 20, textColor);
    };
};