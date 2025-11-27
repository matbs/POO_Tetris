#include <iostream>
#include "include/tetrominos.h"
#include "include/board.h"
#include <thread>
#include <mutex>
#include <atomic>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>

int kbhit(void)
{
        struct termios oldt, newt; // The termios functions describe a general terminal interface that
        // is provided to control asynchronous communications ports
        int ch;
        int oldf;

        tcgetattr(STDIN_FILENO, &oldt); //Pega o que está escrito no terminal e joga para a variável 
        newt = oldt;
        newt.c_lflag &= ~(ICANON | ECHO); // Set the terminal to not work in canonical mode
        tcsetattr(STDIN_FILENO, TCSANOW, &newt); //Set the parameters related to the terminal 
        oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
        fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);

        ch = getchar();

        tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
        fcntl(STDIN_FILENO, F_SETFL, oldf);

        if(ch != EOF)
        {
                ungetc(ch, stdin);
                return 1;
        }

        return 0;
}

std::atomic<bool> isRunning(true); // Dessa maneira eu garanto que as threads possam verificar se o jogo ainda está rodando. Atomic evita condições de corrida.
std::mutex mtx;
//Aqui o tetromino e o board são variaveis compartilhadas entre as threads

void thread1(Board &board, tetromino &t) {
    while (isRunning) {
        std::this_thread::sleep_for(std::chrono::milliseconds(500)); // Controla a velocidade de queda do tetromino
        std::lock_guard<std::mutex> lock(mtx);
        t.moveTetrominoDown();
        if (!board.checkCollision(t)) { //Collision check for downward movement. When colision is false, piece can move down.
            // Atualiza posição global do tetromino
            Points pos = t.getGlobalPosition();
            t.setGlobalPosition(pos.x, pos.y); // Move o tetromino para baixo
            // (Normalmente, a peça seria desenhada aqui)
            std::cout << "Tetromino caiu para y = " << pos.y << std::endl;
            board.print(t);
        } else {
            // Não pode mais descer, peça colide/fixa no tabuleiro
            std::cout << "Colisão detectada. Tetromino parou em y = " << t.getGlobalPosition().y << std::endl;
            board.placePiece(t); // Fixar o tetromino atual no tabuleiro
            board.clearLines(); // Limpar linhas completas, se houver
            //board.checkGamerOver(); // Verifica se o jogo acabou
            board.spawnTetromino();
            t = board.getCurrentTetromino(); // Pega o novo tetromino
            if (board.checkGameOverSpawn(t)) {
                std::cout << "Game Over!" << std::endl;
                        board.print(t);
                isRunning = false; // Sinaliza para as threads pararem
            }
        }
    }
}

void thread2(Board &board, tetromino &t) {
    while (isRunning) {

        if (!kbhit()) continue;

        int c1 = getchar();
        if (c1 != 27) continue; // não é ESC → ignora

        int c2 = getchar();
        if (c2 != '[') continue; // não é sequência de flecha → ignora

        int c3 = getchar(); // 'A' 'B' 'C' 'D'

        std::lock_guard<std::mutex> lock(mtx);

        switch(c3) {
            case 'A': // UP
                t.rotateTetrominoCCW();
                if (board.checkCollision(t)) t.rotateTetrominoCW();
                break;

            case 'B': // DOWN
                t.moveTetrominoDown();
                if (board.checkCollision(t)) t.moveTetromino(0, -1);
                break;

            case 'C': // RIGHT
                t.moveTetromino(1, 0);
                if (board.checkCollision(t)) t.moveTetromino(-1, 0);
                break;

            case 'D': // LEFT
                t.moveTetromino(-1, 0);
                if (board.checkCollision(t)) t.moveTetromino(1, 0);
                break;
        }
    }
}


int main() {
    Board board;
    board.spawnTetromino(); // Gera o primeiro tetromino

    tetromino t = board.getCurrentTetromino();

    board.print(t);
    std::thread t1(thread1, std::ref(board), std::ref(t));
    std::thread t2(thread2, std::ref(board), std::ref(t));

    t1.join();
    t2.join();

    return 0;
}

// // #include <iostream>
// // #include "include/tetrominos.h"
// // #include "include/board.h"

// // int main() {
// //     tetromino t;

// //     std::cout << "Type" << t.getType() << std::endl;

// //     int matrix[4][4];

// //     int count = 4; // Number of rotations to test

// //     for (size_t i = 0; i < count; i++)
// //     {
// //         const Points* blocks = t.getBlock();

// //         std::cout << "Blocks" << blocks[0].x << "," << blocks[0].y << " | "
// //                   << blocks[1].x << "," << blocks[1].y << " | "
// //                   << blocks[2].x << "," << blocks[2].y << " | "
// //                   << blocks[3].x << "," << blocks[3].y << std::endl;

// //         for(int i = 0; i < 4; i++) {
// //             for(int j = 0; j < 4; j++) {
// //                 matrix[i][j] = 0;
// //             }
// //         }

// //         for(int b = 0; b < 4; b++){
// //               matrix[int(blocks[b].y)][int(blocks[b].x)] = 1; // Marcar onde está cada bloco  
// //         }

// //         t.rotateTetrominoCW();

// //         std::cout << "Matrix after rotation " << i+1 << ":" << std::endl;

// //         for (int i = 0; i < count; i++)
// //         {
// //             for(int j = 0; j < count; j++) {
// //                 std::cout << matrix[i][j] << " ";
// //             }
// //             std::cout << std::endl;
// //         }
// //         std::cout << "----" << std::endl;
// //     }
    
// //     return 0;
// // }

// while (isRunning) {
//     if (getchar() == '\x1B') { // if the first value is esc
//         getchar(); // skip the [
//         char value = getchar(); // the real value
//         {
//             switch(value) {
//                     case 'A':
//                         t.rotateTetrominoCCW();
//                         if (board.checkCollision(t)) t.rotateTetrominoCW();
//                         break;
//                     case 'C':
//                         t.moveTetromino(1, 0);
//                         if (board.checkCollision(t)) t.moveTetromino(-1, 0);
//                         break;
//                     case 'D':
//                         t.moveTetromino(-1, 0);
//                         if (board.checkCollision(t)) t.moveTetromino(1, 0);
//                         break;
//                     case 'B':
//                         t.rotateTetrominoCW();
//                         if (board.checkCollision(t)) t.rotateTetrominoCCW();
//                         break;
//                 }
//             }
//         }

//         t.moveTetrominoDown();
//         if (!board.checkCollision(t)) { //Collision check for downward movement. When colision is false, piece can move down.
//             // Atualiza posição global do tetromino
//             Points pos = t.getGlobalPosition();
//             t.setGlobalPosition(pos.x, pos.y); // Move o tetromino para baixo
//             // (Normalmente, a peça seria desenhada aqui)
//             std::cout << "Tetromino caiu para y = " << pos.y << std::endl;
//             board.print(t);
//         } else {
//             // Não pode mais descer, peça colide/fixa no tabuleiro
//             std::cout << "Colisão detectada. Tetromino parou em y = " << t.getGlobalPosition().y << std::endl;
//             // Aqui normalmente você fixaria o tetromino no grid e geraria um novo com board.spawnTetromino()
//             board.placePiece(t); // Fixar o tetromino atual no tabuleiro
//             board.clearLines(); // Limpar linhas completas, se houver
//             //board.checkGamerOver(); // Verifica se o jogo acabou
//             board.spawnTetromino();
//             t = board.getCurrentTetromino(); // Pega o novo tetromino
//             if (board.checkGameOverSpawn(t)) {
//                 std::cout << "Game Over!" << std::endl;
//                         board.print(t);
//                 isRunning = false; // Sinaliza para as threads pararem
//             }
//         }
//     }