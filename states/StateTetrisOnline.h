#pragma once
#include "IState.h"
#include "StateTetrisMultiplayer.h"
#include "../controllers/ControllerTetris.h"
#include "../views/ViewerTetris.h"
#include "../controllers/ControllerServer.h"

enum class OnlinePhase {
    SELECT_ROLE,   
    WAITING_HOST,   
    TYPE_IP,        
    PLAYING,
    GAME_OVER        
};

class StateTetrisOnline : public StateTetrisMultiplayer {
private:
    ControllerServer network;
    
    OnlinePhase currentPhase = OnlinePhase::SELECT_ROLE;
    bool isHost;              
    
    char ipInput[16] = "127.0.0.1";
    int ipLetterCount = 9;

    void SyncGame();

public:
    StateTetrisOnline();
    ~StateTetrisOnline();

    std::unique_ptr<IState> Update() override;
    void Exit() override;
};