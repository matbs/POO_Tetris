# 🕹️ Tetris OOP Implementation
Object-Oriented Programming (OOP) implementation of the classic Tetris game.

---

## ⚙️ Prerequisites

This project requires the **[raylib](https://www.raylib.com/)** library for graphics and input.

### Raylib Installation

You must have the raylib development libraries installed on your system.
    Please refer to the official [Raylib Wiki](https://github.com/raysan5/raylib/) for detailed installation instructions.

---

## 🚀 Build and Run

Use the following command in your terminal to compile the source code and run the game.

```bash
g++ ./*/*.cpp main.cpp -lraylib -lGL -lm -lpthread -ldl -lrt -lX11 -o game && ./game