#pragma once

#include "IState.h"
#include "StateTetris.h"
#include "../controllers/ControllerTetris.h"
#include "../views/ViewerTetris.h"
#include <memory>
#include <thread>

class StateTetrisMultiplayer : public IState {
public:
    StateTetrisMultiplayer() {}
    StateTetrisMultiplayer(const IState & other) {}
    void Enter() override;
    std::unique_ptr<IState> Update() override;
    void Exit() override;
    ControllerTetris controllerTetris1;
    ControllerTetris controllerTetris2;
    ViewerTetris* viewer1;
    ViewerTetris* viewer2;
    int winner;
private:
    int score;
    std::mutex scoreMutex;
};