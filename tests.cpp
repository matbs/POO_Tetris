#include <stdio.h>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include <iostream>

int kbhit(void)
{
        struct termios oldt, newt; // The termios functions describe a general terminal interface that
        // is provided to control asynchronous communications ports
        int ch;
        int oldf;

        tcgetattr(STDIN_FILENO, &oldt); //Pega o que está escrito no terminal e joga para a variável 
        newt = oldt;
        newt.c_lflag &= ~(ICANON | ECHO); // Set the terminal to work in canonical mode
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

int main(void)
{       
        while(!kbhit()) puts("Press a key!  ");
        if (getchar() == '\x1B') { // if the first value is escS
                getchar(); // skip the [
                char value = getchar(); // the real value
                {
                        switch(value) {
                        case 'A':
                        std::cout << "ARROW UP PRESSED" << "\n";
                        break;
                        case 'D':
                        std::cout << "ARROW LEFT PRESSED" << "\n";
                        break;
                        case 'C':
                        std::cout << "ARROW RIGHT PRESSED" << "\n";
                        break;
                        case 'B':
                        std::cout << "ARROW DOWN PRESSED" << "\n";
                        break;
                        }
                }
        }
        return 0;
}//main


