// Code for describing tetromino movements would go here.
#include "tetrominosMov.h"

void moveTetromino(tetromino &t, int dx, int dy) {
//There's a missing constraint here to avoid moving upward
    for (int i = 0; i < 4; ++i) {
        t.blocks[i].x += dx;
        t.blocks[i].y += dy;
    }
}
void rotateTetromino(tetromino &t, bool clockwise) {

    for (int i = 0; i < 4; ++i) {
        int x = t.blocks[i].x;
        int y = t.blocks[i].y;
        if (clockwise) {
            t.blocks[i].x = -y;
            t.blocks[i].y = x;
        } else {
            t.blocks[i].x = y;
            t.blocks[i].y = -x;
        }
    }
}