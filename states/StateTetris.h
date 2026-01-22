#pragma once

#include "IState.h"
#include "StateTetris.h"
#include "../controllers/ControllerTetris.h"
#include "../views/ViewerTetris.h"
#include <memory>
#include <thread>

class StateTetris : public IState {
public:
    StateTetris() {}
    StateTetris(const IState & other) {}
    void Enter() override;
    std::unique_ptr<IState> Update() override;
    void Exit() override;
    ControllerTetris controllerTetris;
    ViewerTetris* viewer;
    std::thread controllerThread;

private:
    int score;
    std::mutex scoreMutex;
};