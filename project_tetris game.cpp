#include <iostream>
#include <conio.h>
#include <windows.h>
#include <cstring>

using namespace std;

// Dimensions of the Tetris grid.
const int width = 10, height = 20;

// Grid for the Tetris game.
int grid[height][width] = {0};

// Current Tetromino.
int currentPiece[4][4] = {0};

// Position of the current Tetromino.
int posX = 0, posY = 0;

// Display the grid.
void displayGrid() {
    system("cls");  // Clear the console screen.
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            if (grid[i][j] == 1 || 
                (i >= posY && i < posY + 4 && j >= posX && j < posX + 4 && currentPiece[i - posY][j - posX] == 1)) {
                cout << "# ";
            } else {
                cout << ". ";
            }
        }
        cout << endl;
    }
}

// Check for collisions.
bool isColliding() {
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            if (currentPiece[i][j] == 1) {
                int x = posX + j, y = posY + i;
                if (x < 0 || x >= width || y >= height || (y >= 0 && grid[y][x] == 1)) {
                    return true;
                }
            }
        }
    }
    return false;
}

// Lock the piece into the grid.
void lockPiece() {
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            if (currentPiece[i][j] == 1) {
                grid[posY + i][posX + j] = 1;
            }
        }
    }
}

// Move the piece down.
bool moveDown() {
    posY++;
    if (isColliding()) {
        posY--;
        lockPiece();
        return false;
    }
    return true;
}

// Move the piece left.
void moveLeft() {
    posX--;
    if (isColliding()) posX++;
}

// Move the piece right
void moveRight() {
    posX++;
    if (isColliding()) posX--;
}

// Rotate the piece.
void rotatePiece() {
    int temp[4][4] = {0};
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            temp[j][3 - i] = currentPiece[i][j];
        }
    }
    memcpy(currentPiece, temp, sizeof(temp));
    if (isColliding()) memcpy(currentPiece, temp, sizeof(temp));
}

// Create a new piece.
void createPiece() {
    memset(currentPiece, 0, sizeof(currentPiece));
    int random = rand() % 7;  // 7 possible pieces
    switch (random) {
        case 0:
            currentPiece[1][1] = currentPiece[1][2] = currentPiece[2][1] = currentPiece[2][2] = 1; // Square piece
            break;
        case 1:
            currentPiece[0][0] = currentPiece[1][0] = currentPiece[2][0] = currentPiece[2][1] = 1; // L shape
            break;
        case 2:
            currentPiece[0][0] = currentPiece[0][1] = currentPiece[0][2] = currentPiece[1][1] = 1; // T shape
            break;
        case 3:
            currentPiece[0][1] = currentPiece[0][2] = currentPiece[1][0] = currentPiece[1][1] = 1; // S shape
            break;
        case 4:
            currentPiece[0][0] = currentPiece[0][1] = currentPiece[1][1] = currentPiece[1][2] = 1; // Z shape
            break;
        case 5:
            currentPiece[0][0] = currentPiece[1][0] = currentPiece[2][0] = currentPiece[3][0] = 1; // I shape
            break;
        case 6:
            currentPiece[0][1] = currentPiece[1][1] = currentPiece[2][1] = currentPiece[3][1] = 1; // J shape
            break;
    }
    
    posX = width / 2 - 2;
    posY = 0;
    if (isColliding()) {
        cout << "Game Over!" << endl;
        exit(0);
    }
}

// Remove full lines from the grid.
void removeFullLines() {
    for (int i = 0; i < height; ++i) {
        bool full = true;
        for (int j = 0; j < width; ++j) {
            if (grid[i][j] == 0) {
                full = false;
                break;
            }
        }
        if (full) {
            for (int k = i; k > 0; --k) {
                memcpy(grid[k], grid[k - 1], sizeof(grid[k]));
            }
            memset(grid[0], 0, sizeof(grid[0]));
        }
    }
}

// Game loop.
void gameLoop() {
    createPiece();
    while (true) {
        displayGrid();
        if (_kbhit()) {
            char key = _getch();
            if (key == 'a') moveLeft();
            else if (key == 'd') moveRight();
            else if (key == 's') moveDown();
            else if (key == 'w') rotatePiece();
            else if (key == 'q') exit(0);
        }
        if (!moveDown()) {
            removeFullLines();
            createPiece();
        }
        Sleep(300);  // Control the speed of the game.
    }
}

int main() {
    cout << "**Welcome to Tetris!**" << endl;
    cout << "Use 'a' (left), 'd' (right), 's' (down), 'w' (rotate), 'q' (quit)." << endl;
    cout << "Press any key to start..." << endl;
    _getch();
    gameLoop();
    return 0;
}
