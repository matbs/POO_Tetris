#pragma once
#include "IState.h"
#include "../assets/ScoreManager.h"
#include <cstdio>
#include <vector>

#define COLOR_GOLD   CLITERAL(Color){ 255, 203, 0, 255 }
#define COLOR_SILVER CLITERAL(Color){ 192, 192, 192, 255 }
#define COLOR_BRONZE CLITERAL(Color){ 205, 127, 50, 255 }

class StateRanking : public IState {

    private:
    std::vector<ScoreRecord> topScores;

    public:
        void Enter() override;
        std::unique_ptr<IState> Update() override;
        void Exit() override;
};