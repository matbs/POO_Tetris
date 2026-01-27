#include "StateTetrisOnline.h"
#include "StateMenu.h"
#include <iostream>

StateTetrisOnline::StateTetrisOnline() {
    viewer2 = nullptr;
    viewer1 = nullptr;
    isHost = false;
}

StateTetrisOnline::~StateTetrisOnline() {
    Exit();
}

void StateTetrisOnline::SyncGame() {
    GamePacket myPacket;
    myPacket.score = controllerTetris2.getScore();
    myPacket.linesCleared = controllerTetris2.getLinesCleared();
    myPacket.nextTetromino = controllerTetris2.getNextTetromino();
    myPacket.gameOver = controllerTetris2.isGameOver();
    myPacket.lifes = controllerTetris2.getLifes();

    for(int i=0; i<20; i++) 
        for(int j=0; j<10; j++) 
            myPacket.board[i][j] = controllerTetris2.getCell(i, j);
    
    tetromino t = controllerTetris2.getCurrentTetromino();
    Points pos = t.getGlobalPosition();
    const Points* pts = t.getBlock();

    for(int k=0; k<4; k++) {
        int x = pos.x + pts[k].x;
        int y = pos.y + pts[k].y;
        if(x>=0 && x<10 && y>=0 && y<20) myPacket.board[y][x] = t.getType() + 1;
    }

    network.SendPacket(myPacket);

    GamePacket remotePacket;
    if (network.PollPacket(remotePacket)) {
        controllerTetris1.setScore(remotePacket.score);
        controllerTetris1.setLinesCleared(remotePacket.linesCleared);
        controllerTetris1.setNextTetromino(remotePacket.nextTetromino);
        controllerTetris1.setGameOver(remotePacket.gameOver);
        controllerTetris1.setLifes(remotePacket.lifes);

        for (int i = 0; i < 20; i++)
            for (int j = 0; j < 10; j++)
                controllerTetris1.setCell(i, j, remotePacket.board[i][j]);
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

        DrawButton(btnMenuRect, "MENU", isMouseOver);
        
        EndDrawing();
        
        if (IsKeyPressed(KEY_ESCAPE)) return std::make_unique<StateMenu>();
        return nullptr;
    }
    else if (currentPhase == OnlinePhase::WAITING_HOST) {

        BeginDrawing();
        ClearBackground(BLACK);
        DrawText("WAITING FOR OPPONENT...", 170, 30, 20, LIGHTGRAY);
        DrawText(("YOUR IP: " + network.GetCurrentIP()).c_str(), 200, 60, 20, GRAY);

        viewer2->Draw();

        DrawButton(btnMenuRect, "MENU", isMouseOver);

        EndDrawing();

        GamePacket dummy;
        network.PollPacket(dummy);

        if (network.IsConnected()) {
            currentPhase = OnlinePhase::PLAYING;
            controllerTetris2.resetGame();
        }


        return nullptr;
    }
    else if (currentPhase == OnlinePhase::TYPE_IP) {
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

        DrawButton(btnMenuRect, "MENU", isMouseOver);

        EndDrawing();
    }
    else if (currentPhase == OnlinePhase::PLAYING) {
        controllerTetris2.GameLoop();

        if(IsKeyDown(KEY_LEFT)) controllerTetris2.moveLeft();
        else if(IsKeyDown(KEY_RIGHT)) controllerTetris2.moveRight();
        else if(IsKeyDown(KEY_UP)) controllerTetris2.rotate();
        else if(IsKeyDown(KEY_DOWN)) controllerTetris2.moveDown();
        
        SyncGame();
        BeginDrawing();
        ClearBackground(BLACK);

        viewer2->Draw();
        viewer1->Draw();

        DrawText("YOU", 50, 50, 20, GREEN);
        DrawText("ENEMY", 400, 50, 20, RED);

        DrawButton(btnMenuRect, "MENU", isMouseOver);

        EndDrawing();

        if (controllerTetris2.isGameOver() && controllerTetris1.isGameOver()) {
            currentPhase = OnlinePhase::GAME_OVER;
        }
    }

    else if (currentPhase == OnlinePhase::GAME_OVER) {
        if (controllerTetris2.getScore() > controllerTetris1.getScore()) {
            localWon = true;
        } else if (controllerTetris1.getScore() > controllerTetris2.getScore()) {
            remoteWon = true;
        } else {
            tie = true;
        }
        BeginDrawing();
        ClearBackground(BLACK);

        viewer2->Draw();
        viewer1->Draw();

        DrawEndScreen(controllerTetris2.getScore(), controllerTetris1.getScore(), localWon, remoteWon, tie);
        DrawButton(btnMenuRect, "MENU", isMouseOver);

        EndDrawing();
    }
    
    return nullptr;
}

void StateTetrisOnline::Exit() {
    network.Close();
    if (viewer2) delete viewer2;
    if (viewer1) delete viewer1;
}