#include "InputManager.h"
#include "raylib.h"

void InputManager::ProcessInput() {
    if (IsKeyDown(KEY_LEFT)) {
        action = Action::MOVE_LEFT;
    } else if (IsKeyDown(KEY_RIGHT)) {
        action = Action::MOVE_RIGHT;
    } else if (IsKeyDown(KEY_UP)) {
        action = Action::ROTATE;
    } else if (IsKeyDown(KEY_DOWN)) {
        action = Action::SOFT_DROP;
    } else if (IsKeyDown(KEY_P)) {
        action = Action::PAUSE;
    } else {
        action = Action::NONE;
    }
}