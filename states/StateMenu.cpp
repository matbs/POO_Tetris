#include "StateMenu.h"
#include "StateTetris.h"
#include "StateTetrisMultiplayer.h"
#include <cstdio>
#include <memory>

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
    for (int i = 0; i < MAX_STARS; i++) {
        stars[i].position.y += stars[i].speed * 5.0f; 
        
        if (stars[i].position.y >= GetScreenHeight()) {
            stars[i].position.y = 0.0f;
            stars[i].position.x = (float)GetRandomValue(0, GetScreenWidth()); 
        }
    }

    BeginDrawing();
    
    ClearBackground(BLACK); 

    for (int i = 0; i < MAX_STARS; i++) {
        DrawPixelV(stars[i].position, stars[i].color); 
    }
    
    DrawText("TETRIS", GetScreenWidth() / 2 - 150, GetScreenHeight() / 4, 80, LIGHTGRAY);

    DrawRectangleRounded({(float)(GetScreenWidth() / 2 - 150), (float)(GetScreenHeight() / 2 - 25), 325, 50}, 0.2f, 10, GRAY);
    DrawText("SINGLE PLAYER", GetScreenWidth() / 2 - 110, GetScreenHeight() / 2 - 15, 30, LIGHTGRAY);

    DrawRectangleRounded({(float)(GetScreenWidth() / 2 - 150), (float)(GetScreenHeight() / 2 + 40), 325, 50}, 0.2f, 10, GRAY);
    DrawText("MULTIPLAYER", GetScreenWidth() / 2 - 95, GetScreenHeight() / 2 + 50, 30, LIGHTGRAY);

    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        Vector2 mousePos = GetMousePosition();
        if (mousePos.x >= GetScreenWidth() / 2 - 100 && mousePos.x <= GetScreenWidth() / 2 + 100 &&
            mousePos.y >= GetScreenHeight() / 2 - 25 && mousePos.y <= GetScreenHeight() / 2 + 25) {
            return std::make_unique<StateTetris>();
        } else if (mousePos.x >= GetScreenWidth() / 2 - 100 && mousePos.x <= GetScreenWidth() / 2 + 100 &&
                   mousePos.y >= GetScreenHeight() / 2 + 40 && mousePos.y <= GetScreenHeight() / 2 + 90) {
            return std::make_unique<StateTetrisMultiplayer>();
        }

    }

    EndDrawing();
    return nullptr;
}

void StateMenu::Exit() {}