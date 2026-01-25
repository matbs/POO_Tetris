#pragma once
#include "IState.h"
#include "../controllers/ControllerTetris.h"
#include "../views/ViewerTetris.h"
#include "../controllers/ControllerServer.h"

enum class OnlinePhase {
    SELECT_ROLE,   
    WAITING_HOST,   
    TYPE_IP,        
    PLAYING         
};

class StateTetrisOnline : public IState {
private:
    ControllerTetris localController;
    ViewerTetris* localViewer;

    ControllerTetris remoteController; 
    ViewerTetris* remoteViewer;

    ControllerServer network;
    
    OnlinePhase currentPhase; 
    bool isHost;              
    
    char ipInput[16] = "127.0.0.1";
    int ipLetterCount = 9;

    void SyncGame();

public:
    StateTetrisOnline();
    ~StateTetrisOnline();

    void Enter() override;
    std::unique_ptr<IState> Update() override;
    void Exit() override;
};