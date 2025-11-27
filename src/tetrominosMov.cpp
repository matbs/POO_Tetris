// // Code for describing tetromino movements would go here.
// // adjust the include to the actual header location (common layout: headers in ../include/)
// #include "../include/tetrominosMov.h"

// void rotateTetromino(tetromino &t, bool clockwise) {
// // Rotation around the origin (0,0)
//     for (int i = 0; i < 4; ++i) {
//         int x = t.blocks[i].x;
//         int y = t.blocks[i].y;
//         if (clockwise) {
//             t.blocks[i].x = -y;
//             t.blocks[i].y = x;
//         } else {
//             t.blocks[i].x = y;
//             t.blocks[i].y = -x;
//         }
//     }
// }