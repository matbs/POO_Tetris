#pragma once
#include "IState.h"
#include <cstdio>

struct Star {
    Vector2 position;
    Color color;
    float speed;
};

class StateMenu : public IState {
private:
    static const int MAX_STARS = 250;
    Star stars[MAX_STARS];

public:
    void Enter() override;
    std::unique_ptr<IState> Update() override;
    void Exit() override;
};