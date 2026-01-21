#pragma once

#include "IState.h"
#include "StateTetris.h"
#include "../controllers/ControllerTetris.h"
#include <memory>
#include <thread>

class StateTetris : public IState {
public:
    StateTetris() {}
    StateTetris(const IState & other) {}
    void Enter() override;
    std::unique_ptr<IState> Update() override;
    void Exit() override;
private:
    ControllerTetris controllerTetris;
    std::thread controllerThread;

    int score;
    std::mutex scoreMutex;
};