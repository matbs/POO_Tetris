# 🕹️ Tetris OOP Implementation
A C++ implementation of the classic Tetris game using Object-Oriented Programming principles.

**Engine:** [raylib](https://www.raylib.com/)

## 🐧 Build for Linux (Native)
These instructions allow you to compile the game to run on Linux machines.

### 1. Install System Dependencies

```bash
sudo apt update
sudo apt install build-essential git libasound2-dev libx11-dev libxrandr-dev libxi-dev libgl1-mesa-dev libglu1-mesa-dev libxcursor-dev libxinerama-dev libwayland-dev libxkbcommon-dev
```

### 2. Setup local Raylib

```bash
wget [https://github.com/raysan5/raylib/releases/download/5.0/raylib-5.0_linux_amd64.tar.gz](https://github.com/raysan5/raylib/releases/download/5.0/raylib-5.0_linux_amd64.tar.gz)

tar -xvzf raylib-5.0_linux_amd64.tar.gz
mkdir -p raylib_linux
cp -r raylib-5.0_linux_amd64/include raylib_linux/
cp -r raylib-5.0_linux_amd64/lib raylib_linux/

rm raylib-5.0_linux_amd64.tar.gz
rm -rf raylib-5.0_linux_amd64
```

### 3. Compile and run

```bash
g++ ./*/*.cpp main.cpp -o game_linux \
    -I./raylib_linux/include \
    ./raylib_linux/lib/libraylib.a \
    -lGL -lm -lpthread -ldl -lrt -lX11 && ./game_linux
```

## 🪟 Build for Windows (Cross-Compilation)

These instructions allow you to generate a .exe file on Linux that can be sent to and played on Windows.

### 1. Install MinGW Compiler
```bash
sudo apt install mingw-w64
```

### 2. Setup Local Raylib (Windows)
```bash
# Download Raylib 5.0 for Windows (MinGW)
wget [https://github.com/raysan5/raylib/releases/download/5.0/raylib-5.0_win64_mingw-w64.zip](https://github.com/raysan5/raylib/releases/download/5.0/raylib-5.0_win64_mingw-w64.zip)

# Extract and Organize
unzip -o raylib-5.0_win64_mingw-w64.zip
mkdir -p raylib_win
cp -r raylib-5.0_win64_mingw-w64/include raylib_win/
cp -r raylib-5.0_win64_mingw-w64/lib raylib_win/

# Cleanup
rm raylib-5.0_win64_mingw-w64.zip
rm -rf raylib-5.0_win64_mingw-w64
```

### 3. Compile for Windows
```bash
x86_64-w64-mingw32-g++ ./*/*.cpp main.cpp -o game.exe \
    -I./raylib_win/include \
    -L./raylib_win/lib \
    -lraylib -lgdi32 -lwinmm -lopengl32 -lws2_32 -static
```

## Raylib Installed

If you have already installed raylib, use the following command in your terminal to compile the source code and run the game.
    Please refer to the official [Raylib Wiki](https://github.com/raysan5/raylib/) for detailed installation instructions.

### Linux
```bash
g++ ./*/*.cpp main.cpp -lraylib -lGL -lm -lpthread -ldl -lrt -lX11 -o game && ./game
```
