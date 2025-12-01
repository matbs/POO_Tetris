#pragma once
#include "IState.h"
#include <cstdio>

class StateMenu : public IState {
public:
    void Enter() override;
    std::unique_ptr<IState> Update() override;
    void Exit() override;
};