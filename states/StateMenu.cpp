#include "StateMenu.h"
#include "StateRanking.h"
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
            return std::make_unique<StateRanking>();
        }
    }

    BeginDrawing();
    
    ClearBackground(BLACK); 

    for (int i = 0; i < MAX_STARS; i++) {
        DrawPixelV(stars[i].position, stars[i].color); 
    }
    
    DrawText("TETRIS", GetScreenWidth() / 2 - 150, GetScreenHeight() / 4, 80, LIGHTGRAY);

    DrawButton(btnSingle, "SINGLE PLAYER", overSingle);
    DrawButton(btnMulti, "MULTIPLAYER", overMulti);
    DrawButton(btnOnline, "ONLINE PLAY", overOnline);
    DrawButton(btnRank, "RANKING", overRank);

    EndDrawing();
    return nullptr;
}

void StateMenu::Exit() {}