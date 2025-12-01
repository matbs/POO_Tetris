#pragma once

enum class Action {
    MOVE_LEFT,
    MOVE_RIGHT,
    ROTATE,
    SOFT_DROP,
    HARD_DROP,
    PAUSE,
    NONE
};

class InputManager {
public:
    InputManager() {}
    ~InputManager();

    void ProcessInput();
private:
    Action action;
};