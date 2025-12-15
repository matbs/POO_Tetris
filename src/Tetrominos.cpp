// Code for describing tetromino movements would go here.
// adjust the include to the actual header location (common layout: headers in ../include/)
// https://tetris.fandom.com/wiki/Super_Rotation_System?file=SRS-pieces.png
#include "../include/Tetrominos.h"
#include <random>

int randomSeven() {
    static std::random_device rd; 
    static std::mt19937 gen(rd()); 
    static std::uniform_int_distribution<int> dist(0, 6); 
    return dist(gen);
}

tetromino::tetromino() {
    // Default constructor initializes a random tetromino
    int random = randomSeven();
    
    switch (random)
    {
    case 0:
    // I
    // {{{0,1}, {1,1}, {2,1}, {3,1}}, 0},
        type = 0;
        positionToBoard = {1,1};
        pivot = {1.5,1.5};
        setBlock(0, 0, 1);
        setBlock(1, 1, 1);
        setBlock(2, 2, 1);
        setBlock(3, 3, 1);
        break;
    case 1:
    //  J
    //  {{{0,0}, {0,1}, {1,1}, {2,1}}, 1},
        type = 1;
        positionToBoard = {0,0};
        pivot = {1,1};
        setBlock(0, 0, 0);
        setBlock(1, 0, 1);
        setBlock(2, 1, 1);
        setBlock(3, 2, 1);
        break;
    case 2:
        // L
        // {{{2,0}, {0,1}, {1,1}, {2,1}}, 2},
        type = 2;
        positionToBoard = {0,0};
        pivot = {1,1};
        setBlock(0, 2, 0);
        setBlock(1, 0, 1);
        setBlock(2, 1, 1);
        setBlock(3, 2, 1);
        break;
    case 3:
        // O
        // {{{1,0}, {2,0}, {1,1}, {2,1}}, 3},   
        type = 3;
        positionToBoard = {2,0};
        pivot = {0,1};
        setBlock(0, 1, 0);
        setBlock(1, 2, 0);
        setBlock(2, 1, 1);
        setBlock(3, 2, 1);
        break;
    case 4:
        // S
        // {{{1,0}, {2,0}, {0,1}, {1,1}}, 4},
        type = 4;
        positionToBoard = {0,0};
        pivot = {1,1};
        setBlock(0, 1, 0);
        setBlock(1, 2, 0);
        setBlock(2, 0, 1);
        setBlock(3, 1, 1);
        break;
    case 5:
        // T
        // {{{1,0}, {0,1}, {1,1}, {2,1}}, 5},
        type = 5;
        positionToBoard = {0,0};
        pivot = {1,1};
        setBlock(0, 1, 0);
        setBlock(1, 0, 1);
        setBlock(2, 1, 1);
        setBlock(3, 2, 1);
        break;
    case 6:
        // Z
        // {{{0,0}, {1,0}, {1,1}, {2,1}}, 6}
        type = 6;
        positionToBoard = {1,1}; //Estava em zero,zero
        pivot = {1,1};
        setBlock(0, 0, 0);
        setBlock(1, 1, 0);
        setBlock(2, 1, 1);
        setBlock(3, 2, 1);
        break;
    }

}

const Points* tetromino::getBlock() const {
    return blocks;
}

void tetromino::setBlock(int i, int x, int y) {
    blocks[i].x = x;
    blocks[i].y = y;
}

Points tetromino::getGlobalPosition() const {
    return positionToBoard;
}

void tetromino::setGlobalPosition(int x, int y) {
    positionToBoard.x = x;
    positionToBoard.y = y;
}


void tetromino::rotateTetrominoCW() {
    if (type == 3) return; // O does not rotate

    const Points* b = this->getBlock();
    float pivotX = this->pivot.x;
    float pivotY = this->pivot.y;

    Points original[4];
    for (int i = 0; i < 4; i++) {
        original[i] = b[i];
    }

    for (int i = 0; i < 4; i++) {
        float relX = original[i].x - pivotX;
        float relY = original[i].y - pivotY;

        float rotX =  -relY;
        float rotY = relX;

        this->setBlock(i, round(rotX + pivotX), round(rotY + pivotY));
    }
}

void tetromino::rotateTetrominoCCW() {
    if (type == 3) return; //O does not rotate

    const Points* b = this->getBlock();
    int pivotX = this->pivot.x;
    int pivotY = this->pivot.y;

    Points original[4];
    for (int i = 0; i < 4; i++) {
        original[i] = b[i];
    }

    for (int i = 0; i < 4; i++) {
        int relX = original[i].x - pivotX;
        int relY = original[i].y - pivotY;
        // rotaciona 90°: (x, y) -> (y, -x)

        int rotX = relY;
        int rotY = -relX;

        this->setBlock(i, round(rotX + pivotX), round(rotY + pivotY));
    }
}


void tetromino::moveTetromino(int dx, int dy) {
//  Moves the tetromino by (dx, dy) on the board
// It does not make any verification of collisions or boundaries.
    const Points* b = this->getBlock();
    for (int i = 0; i < 4; i++) {
        int x = b[i].x;
        int y = b[i].y;
        this->setBlock(i, x + dx, y + dy);
    }
}

void tetromino::moveTetrominoDown() {
//  Moves the tetromino by (dx, dy) on the board
// It does not make any verification of collisions or boundaries.
    this->positionToBoard.y += 1;
}