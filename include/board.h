#ifndef BOARD_H
#define BOARD_H

#include "tetrominos.h"
#define BOARD_WIDTH 10
#define BOARD_HEIGHT 20


class Board {
private:
    int grid[BOARD_HEIGHT][BOARD_WIDTH];
    tetromino currentTetromino; 
    tetromino nextTetromino;
    
public:
    Board();
    
    tetromino getCurrentTetromino();
    void setCurrentTetromino(const tetromino& t);
    void print(const tetromino& t) const;
    
    void spawnTetromino(); //Function to create tetromino based on type
    // void initializeBoard(); //Function to initialize the game board
    bool checkCollision(const tetromino& t) const; //Function to detect collisions on the board superpostion or out of bounds
    void placePiece(const tetromino& t); //Function to place tetromino on the board
    void clearLines(); //Function to clear completed lines on the board
    bool checkGameOverSpawn(const tetromino& t) const; //Function to check if any block is in the top row, indicating game over
};

#endif