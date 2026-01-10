#include "ControllerTetris.h"
#include "../include/Tetrominos.h"

ControllerTetris::ControllerTetris() {}

ControllerTetris::~ControllerTetris() {}

void ControllerTetris::GameLoop() {
    while (this->running)
    {
        board.spawnTetromino();
        tetromino current = board.getCurrentTetromino();
        while (!board.checkCollision(current)) {
            for(int i=0;i<4;++i){
                int x = current.getBlock()[i].x + current.getGlobalPosition().x;
                int y = current.getBlock()[i].y + current.getGlobalPosition().y;
                if(x >= 0 && x < 10 && y >= 0 && y < 20)
                    boardCells[y][x] = 0;
            }

            current.moveTetrominoDown();

            for(int i=0;i<4;++i){
                int x = current.getBlock()[i].x + current.getGlobalPosition().x;
                int y = current.getBlock()[i].y + current.getGlobalPosition().y;
                if(x >= 0 && x < 10 && y >= 0 && y < 20)
                    boardCells[y][x] = current.getType() + 1; 
            }

            std::this_thread::sleep_for(std::chrono::milliseconds(500));
        }
        board.placePiece(current);

        board.clearLines();

        for (int i = 0; i < 20; ++i) {
            for (int j = 0; j < 10; ++j) {
                boardCells[i][j] = board.getCell(i, j);
            }
        }

    }
    
}
