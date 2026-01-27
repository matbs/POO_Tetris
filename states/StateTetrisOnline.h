#pragma once
#include "IState.h"
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

    bool tie = false;
    bool localWon = false;
    bool remoteWon = false;

public:
    StateTetrisOnline();
    ~StateTetrisOnline();

    void Enter() override;
    std::unique_ptr<IState> Update() override;
    void Exit() override;
    void DrawWaitOponnent(int localScore, int remoteScore, bool localWon, bool remoteWon, bool tie);
    void DrawEndScreen(int localScore, int remoteScore, bool localWon, bool remoteWon, bool tie);
};