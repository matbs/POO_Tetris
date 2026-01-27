#include "ViewerTetris.h"

ViewerTetris::ViewerTetris(ControllerTetris* c, int x, int y, int bs) 
    : controller(c), boardX(x), boardY(y), blockSize(bs) 
{}

void ViewerTetris::DrawBlock(int x, int y, int size, Color baseColor, int offsetX, int offsetY) {
    const int BORDER_SIZE = 2;
    float factor = 0.3f;

    Color lightColor = {
        (unsigned char)(baseColor.r + (255 - baseColor.r) * factor),
        (unsigned char)(baseColor.g + (255 - baseColor.g) * factor),
        (unsigned char)(baseColor.b + (255 - baseColor.b) * factor),
        baseColor.a
    };
    Color darkColor = {
        (unsigned char)(baseColor.r * factor), 
        (unsigned char)(baseColor.g * factor), 
        (unsigned char)(baseColor.b * factor), 
        baseColor.a
    };
    
    int px = x * size + offsetX;
    int py = y * size + offsetY;
    
    DrawTriangle(
        { (float)(px),          (float)(py + size) },   
        { (float)(px + size),   (float)(py + size) },   
        { (float)(px + size),   (float)(py) },          
        darkColor
    );

    DrawTriangle(
        { (float)(px),          (float)(py) },          
        { (float)(px),          (float)(py + size) },   
        { (float)(px + size),   (float)(py) },          
        lightColor
    );

    DrawRectangle(
        px + BORDER_SIZE, 
        py + BORDER_SIZE, 
        size - 2 * BORDER_SIZE, 
        size - 2 * BORDER_SIZE, 
        baseColor
    );
}

void ViewerTetris::DrawHeart(int x, int y, int size, Color color) {
    DrawCircle(x + size / 4, y + size / 4, size / 4, color);
    DrawCircle(x + 3 * size / 4, y + size / 4, size / 4, color);
    DrawTriangle(
        { (float)(x - 1),                  (float)y + (float)size * 0.25f },
        { (float)x + (float)size * 0.5f,   (float)y + (float)size * 0.7f },         
        { (float)(x + 1) + (float)size,          (float)y + (float)size * 0.25f },
        color
    );
}

void ViewerTetris::DrawDisplay(int posX, int posY, int sizeX, int sizeY) {
    DrawRectangle(posX, posY, sizeX, sizeY, BLACK);
    DrawRectangleLines(posX, posY, sizeX, sizeY, MAGENTA);
    DrawRectangleLines(posX - 2, posY - 2, sizeX + 4, sizeY + 4, LIGHTGRAY);
    DrawPixel(posX - 2, posY - 2, DARKGRAY);
}

void ViewerTetris::DrawScore() {
    int scoreY = boardY - 60;
    
    DrawDisplay(boardX, scoreY, BOARD_WIDTH * blockSize, 50);
    DrawText("Score:", boardX + 5, scoreY + 3, 20, LIGHTGRAY);
    
    int score = controller->getScore(); 
    
    char scoreText[20];
    sprintf(scoreText, "%d", score);
    DrawText(scoreText, boardX + 5, scoreY + 23, 20, LIGHTGRAY);
}

void ViewerTetris::DrawStats() {
    int statsY = boardY + (BOARD_HEIGHT * blockSize) + 10;
    int halfWidth = (BOARD_WIDTH * blockSize) / 2 - 5;

    DrawDisplay(boardX, statsY, halfWidth, 4*blockSize);
    DrawText("Lines:", boardX + 5, statsY + 2, 20, LIGHTGRAY);
    
    char linesText[20];
    sprintf(linesText, "%d", controller->getLinesCleared());
    DrawText(linesText, boardX + 5, statsY + 25, 20, LIGHTGRAY);
}

void ViewerTetris::DrawNextPiece() {
    int halfWidth = (BOARD_WIDTH * blockSize) / 2 - 5;
    int nextX = boardX + halfWidth + 10;
    int nextY = boardY + (BOARD_HEIGHT * blockSize) + 10;
    
    DrawDisplay(nextX, nextY, halfWidth, 4*blockSize);

    tetromino next = controller->getNextTetromino();
    const Points* blocks = next.getBlock();
    
    int drawOffsetX = nextX + blockSize*2; 
    int drawOffsetY = nextY + blockSize*2;

    if (next.getType() == 0) {
        drawOffsetY -= blockSize / 3;
        drawOffsetX -= 2*blockSize/3;
    }
    if (next.getType() == 3) {
        drawOffsetY -= blockSize;
        drawOffsetX -= blockSize/3;
    }

    for (int i = 0; i < 4; i++) {
        DrawBlock(blocks[i].x, blocks[i].y, blockSize, colors[next.getType() + 1], drawOffsetX, drawOffsetY);
    }
}

void ViewerTetris::DrawBoard() {
    DrawDisplay(boardX, boardY, BOARD_WIDTH * blockSize, BOARD_HEIGHT * blockSize);
    
    for (int i = 0; i < BOARD_HEIGHT; i++) {
        for (int j = 0; j < BOARD_WIDTH; j++) {
            int tileValue = controller->getCell(i, j);
            if (tileValue != 0) {
                if(tileValue < colors.size())
                    DrawBlock(j, i, blockSize, colors[tileValue], boardX, boardY);
            }
        }
    }

    tetromino current = controller->getCurrentTetromino();
    const Points* blocks = current.getBlock();
    Points pos = current.getGlobalPosition();
    int type = current.getType();

    for(int i=0; i<4; ++i) {
        int x = blocks[i].x + pos.x;
        int y = blocks[i].y + pos.y;
        if(y >= 0) {
             DrawBlock(x, y, blockSize, colors[type + 1], boardX, boardY);
        }
    }
}

void ViewerTetris::DrawLives() {
    int startY = boardY + (BOARD_HEIGHT * blockSize) + 4*blockSize + 20;
    
    DrawDisplay(boardX, startY, BOARD_WIDTH * blockSize, 50);
    
    if(controller->isGameOver()) {
        DrawText("GAME OVER", boardX + 5, startY + 15, 20, RED);
        return;
    }
    DrawText("Player", boardX + 5, startY + 5, 20, LIGHTGRAY);
    for (int i = 0; i < controller->getLifes(); i++) {
        DrawHeart(boardX + 10 + (i * 30), startY + 30, 20, RED);
    }
}

void ViewerTetris::Draw() {
    DrawScore();
    DrawBoard();
    DrawStats();
    DrawNextPiece();
    DrawLives();
}