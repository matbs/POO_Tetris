#include "StateTetrisOnline.h"
#include "StateMenu.h"
#include <iostream>

StateTetrisOnline::StateTetrisOnline() {
    localViewer = nullptr;
    remoteViewer = nullptr;
    currentPhase = OnlinePhase::SELECT_ROLE;
    isHost = false;
}

StateTetrisOnline::~StateTetrisOnline() {
    Exit();
}

void StateTetrisOnline::Enter() {
    localViewer = new ViewerTetris(&localController, 50, 150, 15);
    remoteViewer = new ViewerTetris(&remoteController, 400, 150, 15);
}

void StateTetrisOnline::SyncGame() {
    GamePacket myPacket;
    myPacket.score = localController.getScore();
    myPacket.linesCleared = localController.getLinesCleared();
    myPacket.nextTetromino = localController.getNextTetromino();
    myPacket.gameOver = localController.isGameOver();
    myPacket.lifes = localController.getLifes();

    for(int i=0; i<20; i++) 
        for(int j=0; j<10; j++) 
            myPacket.board[i][j] = localController.getCell(i, j);
    
    tetromino t = localController.getCurrentTetromino();
    Points pos = t.getGlobalPosition();
    const Points* pts = t.getBlock();

    for(int k=0; k<4; k++) {
        int x = pos.x + pts[k].x;
        int y = pos.y + pts[k].y;
        if(x>=0 && x<10 && y>=0 && y<20) myPacket.board[y][x] = t.getType() + 1;
    }

    network.SendPacket(myPacket);

    GamePacket remotePacket;
    while (network.ReceivePacket(remotePacket)) {
        remoteController.setScore(remotePacket.score);
        remoteController.setLinesCleared(remotePacket.linesCleared);
        remoteController.setNextTetromino(remotePacket.nextTetromino);
        remoteController.setGameOver(remotePacket.gameOver);
        remoteController.setLifes(remotePacket.lifes);
        
        for(int i=0; i<20; i++) 
            for(int j=0; j<10; j++) 
                remoteController.setCell(i, j, remotePacket.board[i][j]); 
    }
}

std::unique_ptr<IState> StateTetrisOnline::Update() {
    Rectangle btnMenuRect = { 50, 20, 100, 40 };
    Vector2 mousePos = GetMousePosition();
    
    bool isMouseOver = CheckCollisionPointRec(mousePos, btnMenuRect);
    
    if (isMouseOver && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        return std::make_unique<StateMenu>(); 
    }
    
    if (currentPhase == OnlinePhase::SELECT_ROLE) {
        Rectangle btnHost = { (float)GetScreenWidth()/2 - 100, 300, 200, 50 };
        Rectangle btnJoin = { (float)GetScreenWidth()/2 - 100, 400, 200, 50 };
        
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            if (CheckCollisionPointRec(mousePos, btnHost)) {
                isHost = true;
                if (network.InitServer(12345)) {
                    currentPhase = OnlinePhase::WAITING_HOST;
                }
            } else if (CheckCollisionPointRec(mousePos, btnJoin)) {
                isHost = false;
                currentPhase = OnlinePhase::TYPE_IP;
            }
        }

        BeginDrawing();
        ClearBackground(BLACK);


        DrawText("MULTIPLAYER MODE", GetScreenWidth()/2 - 120, 100, 30, LIGHTGRAY);
        
        DrawRectangleRec(btnHost, CheckCollisionPointRec(mousePos, btnHost) ? LIGHTGRAY : GRAY);
        DrawText("HOST GAME", btnHost.x + 40, btnHost.y + 15, 20, BLACK);

        DrawRectangleRec(btnJoin, CheckCollisionPointRec(mousePos, btnJoin) ? LIGHTGRAY : GRAY);
        DrawText("JOIN GAME", btnJoin.x + 45, btnJoin.y + 15, 20, BLACK);

        Color btnColor = isMouseOver ? LIGHTGRAY : GRAY;
        Color textColor = isMouseOver ? BLACK : WHITE;

        DrawRectangleRec(btnMenuRect, btnColor);
        DrawRectangleLinesEx(btnMenuRect, 2, WHITE);

        int textWidth = MeasureText("MENU", 20);
        int textX = btnMenuRect.x + (btnMenuRect.width - textWidth) / 2;
        int textY = btnMenuRect.y + (btnMenuRect.height - 20) / 2;
        DrawText("MENU", textX, textY, 20, textColor);
        
        EndDrawing();
        
        if (IsKeyPressed(KEY_ESCAPE)) return std::make_unique<StateMenu>();
        return nullptr;
    }

    if (currentPhase == OnlinePhase::WAITING_HOST) {

        BeginDrawing();
        ClearBackground(BLACK);
        DrawText("WAITING FOR OPPONENT...", 170, 30, 20, LIGHTGRAY);
        DrawText(("YOUR IP: " + network.GetCurrentIP()).c_str(), 200, 60, 20, GRAY);

        localViewer->Draw();

        Color btnColor = isMouseOver ? LIGHTGRAY : GRAY;
        Color textColor = isMouseOver ? BLACK : WHITE;

        DrawRectangleRec(btnMenuRect, btnColor);
        DrawRectangleLinesEx(btnMenuRect, 2, WHITE);

        int textWidth = MeasureText("MENU", 20);
        int textX = btnMenuRect.x + (btnMenuRect.width - textWidth) / 2;
        int textY = btnMenuRect.y + (btnMenuRect.height - 20) / 2;
        DrawText("MENU", textX, textY, 20, textColor);

        EndDrawing();

        GamePacket dummy;
        network.ReceivePacket(dummy); 
        if (network.IsConnected()) {
            currentPhase = OnlinePhase::PLAYING;
            localController.resetGame();
        }

        return nullptr;
    }

    if (currentPhase == OnlinePhase::TYPE_IP) {
        int key = GetCharPressed();
        while (key > 0) {
            if ((key >= 46 && key <= 57) && (ipLetterCount < 15)) {
                ipInput[ipLetterCount] = (char)key;
                ipInput[ipLetterCount+1] = '\0';
                ipLetterCount++;
            }
            key = GetCharPressed();
        }
        if (IsKeyPressed(KEY_BACKSPACE)) {
            ipLetterCount--;
            if (ipLetterCount < 0) ipLetterCount = 0;
            ipInput[ipLetterCount] = '\0';
        }
        if (IsKeyPressed(KEY_ENTER)) {
            if (network.ConnectClient(ipInput, 12345)) {
                currentPhase = OnlinePhase::PLAYING;
            }
        }

        BeginDrawing();

        ClearBackground(BLACK);

        DrawText("ENTER HOST IP", 200, 250, 30, GREEN);
        DrawRectangle(200, 330, 200, 30, LIGHTGRAY);
        DrawText(ipInput, 210, 335, 20, BLACK);
        DrawText("Press ENTER to Connect", 200, 380, 20, GRAY);

        EndDrawing();
    }

    localController.GameLoop();

    if(IsKeyDown(KEY_LEFT)) localController.moveLeft();
    else if(IsKeyDown(KEY_RIGHT)) localController.moveRight();
    else if(IsKeyDown(KEY_UP)) localController.rotate();
    else if(IsKeyDown(KEY_DOWN)) localController.moveDown();
    
    SyncGame();
    BeginDrawing();
    ClearBackground(BLACK);

    localViewer->Draw();
    remoteViewer->Draw();

    DrawText("YOU", 50, 50, 20, GREEN);
    DrawText("ENEMY", 400, 50, 20, RED);

if (localController.isGameOver() || remoteController.isGameOver()) {
    int localScore = localController.getScore();
    int remoteScore = remoteController.getScore();
    bool localWon = false;
    bool remoteWon = false;
    bool tie = false;
    
    if (localController.isGameOver() && remoteController.isGameOver()) {
        if (localScore > remoteScore) {
            localWon = true;
        } else if (remoteScore > localScore) {
            remoteWon = true;
        } else {
            tie = true;
        }
    } else if (localController.isGameOver()) {
        remoteWon = true;
    } else if (remoteController.isGameOver()) {
        localWon = true;
    }

    if(localController.isGameOver() && !remoteController.isGameOver()){
        DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), {0, 0, 0, 220});
        int panelWidth = 400;
        int panelHeight = 200;
        int panelX = (GetScreenWidth() - panelWidth) / 2;
        int panelY = (GetScreenHeight() - panelHeight) / 2;
        
        DrawRectangleLines(panelX, panelY, panelWidth, panelHeight, LIGHTGRAY);
        DrawRectangleLines(panelX + 2, panelY + 2, panelWidth - 4, panelHeight - 4, DARKGRAY);
        
        const char* text = "YOU LOST! WAITING FOR OPPONENT...";
        int fontSize = 20;
        int textWidth = MeasureText(text, fontSize);
        int textX = panelX + (panelWidth - textWidth) / 2;
        int textY = panelY + (panelHeight - fontSize) / 2;
        
        DrawText(text, textX, textY, fontSize, RED);
    }
    

    if (localController.isGameOver() && remoteController.isGameOver())
    {
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

        int linesY = scoresY + 70;
        DrawText(TextFormat("Lines: %d", localController.getLinesCleared()), leftScoreX, linesY, 18, {200, 200, 200, 200});
        DrawText(TextFormat("Lines: %d", remoteController.getLinesCleared()), rightScoreX, linesY, 18, {200, 200, 200, 200});

        DrawRectangle(panelX + 40, panelY + panelHeight - 70, panelWidth - 80, 1, {60, 60, 60, 100});

        int instructionsY = panelY + panelHeight - 40;

        int restartWidth = MeasureText("R - Restart Match", 20);
        int menuWidth = MeasureText("ESC - Main Menu", 20);
        int totalWidth = restartWidth + 30 + menuWidth;
        
        DrawText("ESC - Exit Game", 
                panelX + (panelWidth - totalWidth) / 2 + 120, 
                instructionsY, 
                20, 
                LIGHTGRAY);
    }
}

    Color btnColor = isMouseOver ? LIGHTGRAY : GRAY;
    Color textColor = isMouseOver ? BLACK : WHITE;

    DrawRectangleRec(btnMenuRect, btnColor);
    DrawRectangleLinesEx(btnMenuRect, 2, WHITE);

    int textWidth = MeasureText("MENU", 20);
    int textX = btnMenuRect.x + (btnMenuRect.width - textWidth) / 2;
    int textY = btnMenuRect.y + (btnMenuRect.height - 20) / 2;
    DrawText("MENU", textX, textY, 20, textColor);

    EndDrawing();
    
    return nullptr;
}

void StateTetrisOnline::Exit() {
    network.Close();
    if (localViewer) delete localViewer;
    if (remoteViewer) delete remoteViewer;
}