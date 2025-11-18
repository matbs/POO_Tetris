//There are 7 standard tetrominos in Tetris: I, J, L, O, S, T, and Z.
//Each tetromino is represented by a struct containing its blocks' coordinates and type.
// Notice that the tetromino dynamic behaviors don't influence the board representation directly.

#ifndef TETROMINOS_H
#define TETROMINOS_H    

struct Points
{
    int x;
    int y;
};

struct tetromino
{
    Points blocks[4]; // 4 blocks making up the tetromino
    int type;          // 0-6 representing the tetromino type
    //int rotation;  // Current rotation state (0-3) - Idk if needed
};

const tetromino TETROMINOS[7] = {
    // I
    {{{0, 1}, {1, 1}, {2, 1}, {3, 1}}, 0},
    // J
    {{{0, 0}, {0, 1}, {1, 1}, {2, 1}}, 1},
    // L
    {{{2, 0}, {0, 1}, {1, 1}, {2, 1}}, 2},
    // O
    {{{1, 0}, {2, 0}, {1, 1}, {2, 1}}, 3},
    // S
    {{{1, 0}, {2, 0}, {0, 1}, {1, 1}}, 4},
    // T
    {{{1, 0}, {0, 1}, {1, 1}, {2, 1}}, 5},
    // Z
    {{{0, 0}, {1, 0}, {1, 1}, {2, 1}}, 6}
};

#endif // TETROMINOS_H
