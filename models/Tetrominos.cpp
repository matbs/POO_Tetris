// https://tetris.fandom.com/wiki/Super_Rotation_System?file=SRS-pieces.png
#include "Tetrominos.h"
#include <random>

int randomSeven() {
    static std::random_device rd; 
    static std::mt19937 gen(rd()); 
    static std::uniform_int_distribution<int> dist(0, 6); 
    return dist(gen);
}

tetromino::tetromino() {
    type = randomSeven();   // Pick one of the 7 tetromino types
    rotationState = 0;      // Initial rotation state

    // All pieces use integer grid coordinates
    // Pivot at (1,1) for 3x3 pieces
    // I-piece uses 4x4, O-piece does not rotate

    switch (type) {

        case 0: // I
            blocks[0] = {-1, 0};
            blocks[1] = {0, 0};
            blocks[2] = {1, 0};
            blocks[3] = {2, 0};
            positionToBoard = {3, 0};
            break;

        case 1: // J
            blocks[0] = {-1, -1};
            blocks[1] = {-1, 0};
            blocks[2] = {0, 0};
            blocks[3] = {1, 0};
            positionToBoard = {4, 0};
            break;

        case 2: // L
            blocks[0] = {-1, 0};
            blocks[1] = {0, 0};
            blocks[2] = {1, 0};
            blocks[3] = {1, -1};
            positionToBoard = {4, 0};
            break;

        case 3: // O
            blocks[0] = {0, 0};
            blocks[1] = {1, 0};
            blocks[2] = {0, 1};
            blocks[3] = {1, 1};
            positionToBoard = {4, 0};
            break;

        case 4: // S
            blocks[0] = {-1, 0};
            blocks[1] = {0, 0};
            blocks[2] = {0, -1};
            blocks[3] = {1, -1};
            positionToBoard = {4, 0};
            break;

        case 5: // T
            blocks[0] = {-1, 0};
            blocks[1] = {0, 0};
            blocks[2] = {1, 0};
            blocks[3] = {0, -1};
            positionToBoard = {4, 0};
            break;

        case 6: // Z 
            blocks[0] = {-1, 0};
            blocks[1] = {0, 0};
            blocks[2] = {0, -1};
            blocks[3] = {1, -1};

            positionToBoard = {4, 0};
            break;
    }
}

void tetromino::moveTetromino(int dx, int dy) {
//  Moves the tetromino by (dx, dy) on the board
// It does not make any verification of collisions or boundaries.
    positionToBoard.x += dx;
    positionToBoard.y += dy;
}


void tetromino::moveTetrominoDown() {
//  Moves the tetromino down on the board
// It does not make any verification of collisions or boundaries.
    positionToBoard.y += 1;
}

void tetromino::rotateTetrominoCW() {
    if (type == 3) return; // O-piece does not rotate

    static const int SRS_positions[7][4][8] = {
        // I-piece (grid 4x4)
        {
            {-1,0, 0,0, 1,0, 2,0},    
            {0,-1, 0,0, 0,1, 0,2},    
            {-2,0, -1,0, 0,0, 1,0},   
            {0,-2, 0,-1, 0,0, 0,1}    
        },
        // J-piece (grid 3x3)
        {
            {-1,-1, -1,0, 0,0, 1,0},
            {0,-1, 1,-1, 0,0, 0,1},
            {-1,0, 0,0, 1,0, 1,1},
            {0,-1, 0,0, -1,1, 0,1}
        },
        // L-piece (grid 3x3)
        {
            {-1,0, 0,0, 1,0, 1,-1},
            {0,-1, 0,0, 0,1, 1,1},
            {-1,0, 0,0, 1,0, -1,1},
            {-1,-1, 0,-1, 0,0, 0,1}
        },
        // O-piece - não usado (não rotaciona)
        {
            {0,0, 1,0, 0,1, 1,1},
            {0,0, 1,0, 0,1, 1,1},
            {0,0, 1,0, 0,1, 1,1},
            {0,0, 1,0, 0,1, 1,1}
        },
        // S-piece (grid 3x3)
        {
            {-1,0, 0,0, 0,-1, 1,-1},
            {0,-1, 0,0, 1,0, 1,1},
            {-1,1, 0,1, 0,0, 1,0},
            {-1,-1, -1,0, 0,0, 0,1}
        },
        // T-piece (grid 3x3)
        {
            {-1,0, 0,0, 1,0, 0,-1},
            {0,-1, 0,0, 0,1, 1,0},
            {-1,0, 0,0, 1,0, 0,1},
            {0,-1, 0,0, 0,1, -1,0}
        },
        // Z-piece (grid 3x3)
        {
            {-1,-1, 0,-1, 0,0, 1,0},
            {0,1, 0,0, 1,0, 1,-1},
            {-1,0, 0,0, 0,1, 1,1},
            {-1,0, -1,1, 0,0, 0,-1}
        }
    };

    int nextState = (rotationState + 1) % 4;

    for (int i = 0; i < 4; i++) {
        blocks[i].x = SRS_positions[type][nextState][i * 2];
        blocks[i].y = SRS_positions[type][nextState][i * 2 + 1];
    }

    rotationState = nextState;
}

const Points* tetromino::getBlock() const {
    return blocks;
}

Points tetromino::getGlobalPosition() const {
    return positionToBoard;
}

void tetromino::setGlobalPosition(int x, int y) {
    positionToBoard.x = x;
    positionToBoard.y = y;
}

const std::vector<Points> tetromino::I_KICKS[4][4] = {
    // 0 -> R
    {
        {}, {{0,0},{-2,0},{1,0},{-2,-1},{1,2}}, {}, {}
    },
    // R -> 2
    {
        {}, {}, {{0,0},{-1,0},{2,0},{-1,2},{2,-1}}, {}
    },
    // 2 -> L
    {
        {}, {}, {}, {{0,0},{2,0},{-1,0},{2,1},{-1,-2}}
    },
    // L -> 0
    {
        {{0,0},{1,0},{-2,0},{1,-2},{-2,1}}, {}, {}, {}
    }
};

const std::vector<Points> tetromino::JLSTZ_KICKS[4][4] = {
    // 0 -> R
    {
        {}, {{0,0},{-1,0},{-1,1},{0,-2},{-1,-2}}, {}, {}
    },
    // R -> 2
    {
        {}, {}, {{0,0},{1,0},{1,-1},{0,2},{1,2}}, {}
    },
    // 2 -> L
    {
        {}, {}, {}, {{0,0},{1,0},{1,1},{0,-2},{1,-2}}
    },
    // L -> 0
    {
        {{0,0},{-1,0},{-1,-1},{0,2},{-1,2}}, {}, {}, {}
    }
};

const std::vector<Points>& tetromino::getSRSKicks(int type, int from, int to) {
    if (type == 0)
        return I_KICKS[from][to];
    else
        return JLSTZ_KICKS[from][to];
}
