#include "IState.h"

void IState::DrawButton(Rectangle rect, const char* text, bool isMouseOver) {
    Color btnColor = isMouseOver ? LIGHTGRAY : GRAY;
    Color textColor = isMouseOver ? BLACK : WHITE;

    DrawRectangleRec(rect, btnColor);
    DrawRectangleLinesEx(rect, 2, WHITE);

    int textWidth = MeasureText(text, 20);
    int textX = rect.x + (rect.width - textWidth) / 2;
    int textY = rect.y + (rect.height - 20) / 2;
    DrawText(text, textX, textY, 20, textColor);
};