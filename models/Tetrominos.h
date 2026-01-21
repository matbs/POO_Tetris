#ifndef TETROMINOS_H
#define TETROMINOS_H    

#include <vector>

struct Points
{
    float x;
    float y;
};

class tetromino {
private:
    Points blocks[4];  // Positions of the 4 blocks relative to the pivot
    Points positionToBoard; // Position of the pivot to the global board

    int type;         // Indicates the type of tetromino (0-6)
    int rotationState; // Indicates the rotation state (0-3)

    static const std::vector<Points> I_KICKS[4][4];
    static const std::vector<Points> JLSTZ_KICKS[4][4];

public:    
    tetromino();

    void moveTetromino(int dx, int dy); //  Moves the tetromino by (dx, dy) on the board
    void moveTetrominoDown(); //  Moves the tetromino down by 1 on the board

    void rotateTetrominoCW(); // Rotates the tetromino clockwise

    const Points* getBlock() const; // Returns the array of block positions
   
    Points getGlobalPosition() const; // Returns the global position of the tetromino
    void setGlobalPosition(int x, int y); // Sets the global position of the tetromino

    const int getType() const { return type; } // Returns the type of the tetromino 

    static const std::vector<Points>& getSRSKicks(int type, int from, int to);

    int getRotationState() const { return rotationState; }
    void setRotationState(int state) { rotationState = state; }
};

#endif // TETROMINOS_H
