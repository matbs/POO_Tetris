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
    myPacket.gameOver = localController.isGameOver();
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

    if (localController.isGameOver()) {
        DrawText("YOU LOST!", 100, 300, 40, RED);
    }
    if (remoteController.isGameOver()) {
        DrawText("YOU WIN!", 450, 300, 40, GREEN);
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

    if (IsKeyPressed(KEY_ESCAPE)) return std::make_unique<StateMenu>();
    
    return nullptr;
}

void StateTetrisOnline::Exit() {
    network.Close();
    if (localViewer) delete localViewer;
    if (remoteViewer) delete remoteViewer;
}