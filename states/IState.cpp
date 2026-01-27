#include "IState.h"

void IState::DrawButton(Rectangle rect, const char* text, bool isMouseOver) {
    Color btnColor = isMouseOver ? LIGHTGRAY : GRAY;
    Color textColor = isMouseOver ? BLACK : WHITE;

    DrawRectangleRec(rect, btnColor);
    DrawRectangleLinesEx(rect, 2, WHITE);

    int textWidth = MeasureText(text, 20);
    int textX = rect.x + (rect.width - textWidth) / 2;
    int textY = rect.y + (rect.height - 20) / 2;
    DrawText(text, textX, textY, 20, textColor);
};

void IState::DrawEndScreen(int localScore, int remoteScore, bool localWon, bool remoteWon, bool tie) {
        DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), {0, 0, 0, 220});

        int panelWidth = 500;
        int panelHeight = 400;
        int panelX = (GetScreenWidth() - panelWidth) / 2;
        int panelY = (GetScreenHeight() - panelHeight) / 2;

        DrawRectangleLines(panelX, panelY, panelWidth, panelHeight, LIGHTGRAY);
        DrawRectangleLines(panelX + 2, panelY + 2, panelWidth - 4, panelHeight - 4, DARKGRAY);

        int titleWidth = MeasureText("GAME OVER", 32);
        DrawText("GAME OVER", panelX + (panelWidth - titleWidth) / 2, panelY + 20, 32, MAGENTA);

        DrawRectangle(panelX + 30, panelY + 65, panelWidth - 60, 1, {100, 100, 100, 150});

        std::string resultText;
        Color resultColor;
        
        if (tie) {
            resultText = "IT'S A TIE!";
            resultColor = YELLOW;
        } else if (localWon) {
            resultText = "VICTORY!";
            resultColor = GREEN;
        } else {
            resultText = "DEFEAT!";
            resultColor = RED;
        }

        int resultWidth = MeasureText(resultText.c_str(), 36);
        DrawText(resultText.c_str(), panelX + (panelWidth - resultWidth) / 2, panelY + 90, 36, resultColor);

        std::string scoreMessage;
        if (tie) {
            scoreMessage = TextFormat("Both players scored %d points!", localScore);
        } else if (localWon) {
            scoreMessage = TextFormat("You won by %d points!", localScore - remoteScore);
        } else {
            scoreMessage = TextFormat("You lost by %d points!", remoteScore - localScore);
        }
        
        int messageWidth = MeasureText(scoreMessage.c_str(), 20);
        DrawText(scoreMessage.c_str(), panelX + (panelWidth - messageWidth) / 2, panelY + 140, 20, LIGHTGRAY);

        DrawRectangle(panelX + 50, panelY + 170, panelWidth - 100, 1, {80, 80, 80, 100});

        int statsY = panelY + 190;

        DrawText("SCOREBOARD", panelX + (panelWidth - MeasureText("SCOREBOARD", 24)) / 2, statsY, 24, LIGHTGRAY);

        int scoresY = statsY + 40;

        Color localScoreColor = localWon ? GREEN : (tie ? YELLOW : LIGHTGRAY);
        Color remoteScoreColor = remoteWon ? GREEN : (tie ? YELLOW : LIGHTGRAY);

        int scorePanelWidth = 200;
        int leftScoreX = panelX + (panelWidth/2 - scorePanelWidth - 20);
        int rightScoreX = panelX + (panelWidth/2 + 20);

        DrawText("YOUR SCORE", leftScoreX, scoresY, 18, LIGHTGRAY);
        DrawText(TextFormat("%d", localScore), 
                leftScoreX + (scorePanelWidth - MeasureText(TextFormat("%d", localScore), 28)) / 2, 
                scoresY + 30, 28, localScoreColor);

        DrawText("OPPONENT SCORE", rightScoreX, scoresY, 18, LIGHTGRAY);
        DrawText(TextFormat("%d", remoteScore), 
                rightScoreX + (scorePanelWidth - MeasureText(TextFormat("%d", remoteScore), 28)) / 2, 
                scoresY + 30, 28, remoteScoreColor);

        DrawRectangle(panelX + panelWidth/2 - 1, scoresY - 10, 2, 80, {100, 100, 100, 150});

        DrawRectangle(panelX + 40, panelY + panelHeight - 70, panelWidth - 80, 1, {60, 60, 60, 100});
}