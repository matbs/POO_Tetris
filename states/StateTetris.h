#pragma once

#include "IState.h"
#include "StateTetris.h"
#include "../controllers/ControllerTetris.h"
#include <memory>

class StateTetris : public IState {
public:
    StateTetris() {}
    StateTetris(const IState & other) {}
    void Enter() override;
    std::unique_ptr<IState> Update() override;
    void Exit() override;
private:
    ControllerTetris controller;
    int mockBoard[20][10] = {{0,0,0,0,0,0,0,0,0,0}, 
                            {0,0,0,0,0,0,0,0,0,0},
                            {0,0,0,0,0,0,0,0,0,0},
                            {0,0,0,0,0,0,0,0,0,0},
                            {0,0,0,0,0,0,0,0,0,0},
                            {0,0,0,0,0,0,0,0,0,0},
                            {0,0,0,0,0,0,0,0,0,0},
                            {0,0,0,0,0,0,0,0,0,0},
                            {0,0,0,0,0,0,0,0,0,0},
                            {0,0,0,0,0,0,0,0,0,0},
                            {0,0,0,0,0,0,1,1,1,1},
                            {0,0,0,0,0,0,1,1,2,2},
                            {0,0,2,2,3,3,3,3,3,1},
                            {0,0,0,0,0,0,0,0,0,0},
                            {0,0,0,0,0,0,0,0,0,0},
                            {0,0,0,0,0,0,0,0,0,0},
                            {0,0,0,0,0,0,0,0,0,0},
                            {0,0,0,0,0,0,0,0,0,0},
                            {0,0,0,0,0,0,0,0,0,0},
                            {0,0,0,0,0,0,0,0,0,0}};
};