#include "StateRanking.h"
#include "StateMenu.h"
#include "../assets/ScoreManager.h"
#include <cstdio>
#include <memory>

void StateRanking::Enter() { 
    ScoreManager scoreManager;
    topScores = scoreManager.getTopScores();
}

std::unique_ptr<IState> StateRanking::Update() {
    ScoreManager scoreManager;

    Rectangle btnMenuRect = { 50, 20, 100, 40 };
    Vector2 mousePos = GetMousePosition();
    
    bool isMouseOver = CheckCollisionPointRec(mousePos, btnMenuRect);
    
    if (isMouseOver && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        return std::make_unique<StateMenu>(); 
    }

    BeginDrawing();
    
    ClearBackground(BLACK); 

    DrawButton(btnMenuRect, "MENU", isMouseOver);

    DrawText("HALL OF FAME", GetScreenWidth()/2 - MeasureText("HALL OF FAME", 50)/2, 60, 50, GOLD);
    
    int panelWidth = 500;
    int panelHeight = 550;
    int startX = (GetScreenWidth() - panelWidth) / 2;
    int startY = 120;

    DrawRectangleRec({(float)startX, (float)startY, (float)panelWidth, (float)panelHeight}, DARKGRAY);
    DrawRectangleLinesEx({(float)startX, (float)startY, (float)panelWidth, (float)panelHeight}, 2, LIGHTGRAY);

    DrawText("POS", startX + 40, startY + 30, 25, GRAY);
    DrawText("SCORE", startX + 215, startY + 30, 25, GRAY);
    DrawText("LEVEL", startX + panelWidth - MeasureText("LEVEL", 25) - 40, startY + 30, 25, GRAY);
    
    DrawLine(startX + 20, startY + 65, startX + panelWidth - 20, startY + 65, GRAY);

    for (size_t i = 0; i < topScores.size(); ++i) {
        Color rowColor = WHITE;
        int fontSize = 30;

        if (i == 0) { rowColor = COLOR_GOLD; }
        else if (i == 1) { rowColor = COLOR_SILVER; }
        else if (i == 2) { rowColor = COLOR_BRONZE; }

        int y = startY + 85 + (i * 42);

        DrawText(TextFormat("#%d", i + 1), startX + 45, y, fontSize, rowColor);

        DrawText(TextFormat("%06d", topScores[i].score), startX + 220, y, fontSize, rowColor);

        DrawText(TextFormat("%d", topScores[i].level), startX + panelWidth - MeasureText(TextFormat("%d", topScores[i].level), fontSize) - 45, y, fontSize, rowColor);
    }

    EndDrawing();
    return nullptr;
}

void StateRanking::Exit() {}