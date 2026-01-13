//There are 7 standard tetrominos in Tetris: I, J, L, O, S, T, and Z.
//Each tetromino is represented by a struct containing its blocks' coordinates and type.
// Notice that the tetromino dynamic behaviors don't influence the board representation directly.

#ifndef TETROMINOS_H
#define TETROMINOS_H    

struct Points
{
    float x;
    float y;
};

class tetromino {
private:
    Points blocks[4];  // Positions of the 4 blocks
    Points positionToBoard; // Position of the tetromino on the board
    Points pivot;        // Pivot point for rotations
    int type;         // 0 a 6

public:    
    tetromino();

    const Points* getBlock() const;
    void setBlock(int i, int x, int y);
    
    Points getGlobalPosition() const;
    void setGlobalPosition(int x, int y);

    void rotateTetrominoCW();
    void rotateTetrominoCCW();

    void moveTetromino(int dx, int dy);
    void moveTetrominoDown();

    const int getType() const { return type; }
};

#endif // TETROMINOS_H