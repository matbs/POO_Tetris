#pragma once

#include "IState.h"
#include "StateTetris.h"
#include "../controllers/ControllerTetris.h"
#include "../views/ViewerTetris.h"
#include <memory>
#include <thread>

enum class MultiplayerPhase {       
    PLAYING,
    GAME_OVER        
};

class StateTetrisMultiplayer : public IState {
public:

    void Enter() override;
    std::unique_ptr<IState> Update() override;
    void Exit() override;

    ControllerTetris controllerTetris1;
    ControllerTetris controllerTetris2;
    ViewerTetris* viewer1;
    ViewerTetris* viewer2;

    MultiplayerPhase currentPhase = MultiplayerPhase::PLAYING;

    int winner = 0;
};