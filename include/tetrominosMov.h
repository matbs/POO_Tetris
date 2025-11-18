#ifndef TETROMINOSMOV_H
#define TETROMINOSMOV_H

#include "tetrominos.h"

void moveTetromino(tetromino &t, int dx, int dy);
void rotateTetromino(tetromino &t, bool clockwise);

#endif // TETROMINOSMOV_H
