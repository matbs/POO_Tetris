#include "StateMenu.h"
#include "StateTetris.h"
#include "StateTetrisMultiplayer.h"
#include "StateTetrisOnline.h"
#include "../assets/ScoreManager.h"
#include <cstdio>
#include <memory>

void DrawRanking(const std::vector<ScoreRecord>& scores) {
    int startX = GetScreenWidth() - 380;
    int startY = GetScreenHeight() / 4;
    int width  = 340;
    int height = 360;

    DrawRectangleRounded(
        {(float)startX, (float)startY, (float)width, (float)height},
        0.15f, 10, DARKGRAY
    );

    DrawText("TOP 10", startX + 110, startY + 10, 30, RAYWHITE);

    for (size_t i = 0; i < scores.size(); ++i) {
        Color color = LIGHTGRAY;

        if (i == 0) color = GOLD;
        else if (i == 1) color = GOLD;
        else if (i == 2) color = GOLD;

        int y = startY + 60 + i * 28;

        DrawText(
            TextFormat(
                "%2zuº  %5d  Lv %d",
                i + 1,
                scores[i].score,
                scores[i].level
            ),
            startX + 20,
            y,
            22,
            color
        );
    }
}

void StateMenu::Enter() {
    for (int i = 0; i < MAX_STARS; i++) {
        stars[i].position.x = (float)GetRandomValue(0, GetScreenWidth());
        stars[i].position.y = (float)GetRandomValue(0, GetScreenHeight());
        
        stars[i].speed = (float)GetRandomValue(5, 10) / 10.0f; 
       
        unsigned char brightness = (unsigned char)(stars[i].speed * 200 + 55); 
        stars[i].color = {brightness, brightness, brightness, 255};
    }
}

std::unique_ptr<IState> StateMenu::Update() {
    ScoreManager scoreManager;

    for (int i = 0; i < MAX_STARS; i++) {
        stars[i].position.y += stars[i].speed * 5.0f; 
        
        if (stars[i].position.y >= GetScreenHeight()) {
            stars[i].position.y = 0.0f;
            stars[i].position.x = (float)GetRandomValue(0, GetScreenWidth()); 
        }
    }

    Rectangle btnSingle = { (float)(GetScreenWidth() / 2 - 150), (float)(GetScreenHeight() / 2 - 25), 325, 50 };
    Rectangle btnMulti = { (float)(GetScreenWidth() / 2 - 150), (float)(GetScreenHeight() / 2 + 40), 325, 50 };
    Rectangle btnOnline = { (float)(GetScreenWidth() / 2 - 150), (float)(GetScreenHeight() / 2 + 105), 325, 50 };
    Rectangle btnRank = { (float)(GetScreenWidth() / 2 - 150), (float)(GetScreenHeight() / 2 + 170), 325, 50 };

    Vector2 mousePos = GetMousePosition();

    bool overSingle = CheckCollisionPointRec(mousePos, btnSingle);
    bool overMulti = CheckCollisionPointRec(mousePos, btnMulti);
    bool overOnline = CheckCollisionPointRec(mousePos, btnOnline);
    bool overRank = CheckCollisionPointRec(mousePos, btnRank);

    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        if (overSingle) {
            return std::make_unique<StateTetris>();
        } else if (overMulti) {
            return std::make_unique<StateTetrisMultiplayer>();
        } else if (overOnline) {
            return std::make_unique<StateTetrisOnline>();
        } else if (overRank) {
            showingRanking = !showingRanking;
        }
    }

    BeginDrawing();
    
    ClearBackground(BLACK); 

    for (int i = 0; i < MAX_STARS; i++) {
        DrawPixelV(stars[i].position, stars[i].color); 
    }
    
    DrawText("TETRIS", GetScreenWidth() / 2 - 150, GetScreenHeight() / 4, 80, LIGHTGRAY);

    DrawRectangleRec(btnSingle, overSingle ? LIGHTGRAY : GRAY);
    DrawRectangleLinesEx(btnSingle, 2, WHITE);
    int textWidthSingle = MeasureText("SINGLE PLAYER", 30);
    DrawText("SINGLE PLAYER", (int)(btnSingle.x + (btnSingle.width - textWidthSingle) / 2), (int)(btnSingle.y + 10), 30, overSingle ? BLACK : LIGHTGRAY);

    DrawRectangleRec(btnMulti, overMulti ? LIGHTGRAY : GRAY);
    DrawRectangleLinesEx(btnMulti, 2, WHITE);
    int textWidthMulti = MeasureText("MULTIPLAYER", 30);
    DrawText("MULTIPLAYER", (int)(btnMulti.x + (btnMulti.width - textWidthMulti) / 2), (int)(btnMulti.y + 10), 30, overMulti ? BLACK : LIGHTGRAY);

    DrawRectangleRec(btnOnline, overOnline ? LIGHTGRAY : GRAY);
    DrawRectangleLinesEx(btnOnline, 2, WHITE);
    int textWidthOnline = MeasureText("ONLINE PLAY", 30);
    DrawText("ONLINE PLAY", (int)(btnOnline.x + (btnOnline.width - textWidthOnline) / 2), (int)(btnOnline.y + 10), 30, overOnline ? BLACK : LIGHTGRAY);

    DrawRectangleRec(btnRank, overRank ? LIGHTGRAY : GRAY);
    DrawRectangleLinesEx(btnRank, 2, WHITE);
    int textWidthRank = MeasureText("RANKING", 30);
    DrawText("RANKING", (int)(btnRank.x + (btnRank.width - textWidthRank) / 2), (int)(btnRank.y + 10), 30, overRank ? BLACK : LIGHTGRAY);

    if (showingRanking) {
        std::vector<ScoreRecord> topScores = scoreManager.getTopScores();
        DrawRanking(topScores);
    }

    EndDrawing();
    return nullptr;
}

void StateMenu::Exit() {}