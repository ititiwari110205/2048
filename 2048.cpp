#include <iostream>
#include <cstdlib>
#include <ctime>
#include <iomanip>
#include <chrono>
#include <thread>

using namespace std;

const int SIZE = 4;

class Game2048 {
public:
    Game2048();
    void play();

private:
    int board[SIZE][SIZE];
    bool gameOver;

    void initializeBoard();
    void printBoard();
    void generateRandomTile();
    bool moveTiles(char direction);
    bool mergeTiles(char direction);
    bool isGameOver();
    void delay(int milliseconds);
};

Game2048::Game2048() {
    initializeBoard();
    gameOver = false;
}

void Game2048::initializeBoard() {
    for (int i = 0; i < SIZE; ++i) {
        for (int j = 0; j < SIZE; ++j) {
            board[i][j] = 0;
        }
    }

    // Generating two random tiles at the start
    generateRandomTile();
    generateRandomTile();
}

void Game2048::printBoard() {
    system("clear"); // for UNIX/Linux
    // system("cls"); // for Windows

    cout << "-----------------------------" << endl;
    for (int i = 0; i < SIZE; ++i) {
        cout << "|";
        for (int j = 0; j < SIZE; ++j) {
            if (board[i][j] == 0) {
                cout << setw(4) << "|";
            } else {
                cout << setw(3) << board[i][j] << setw(1) << "|";
            }
        }
        cout << endl;
        cout << "-----------------------------" << endl;
    }

    delay(500); // Adjust the delay based on your preference
}

void Game2048::generateRandomTile() {
    int emptyTiles = 0;
    for (int i = 0; i < SIZE; ++i) {
        for (int j = 0; j < SIZE; ++j) {
            if (board[i][j] == 0) {
                emptyTiles++;
            }
        }
    }

    if (emptyTiles == 0) {
        return; // No empty tile available
    }

    int randomPosition = rand() % emptyTiles + 1;
    int count = 0;
    for (int i = 0; i < SIZE; ++i) {
        for (int j = 0; j < SIZE; ++j) {
            if (board[i][j] == 0) {
                count++;
                if (count == randomPosition) {
                    // Generate a 2 or 4 randomly (90% chance for 2, 10% chance for 4)
                    board[i][j] = (rand() % 10 == 0) ? 4 : 2;
                    return;
                }
            }
        }
    }
}

bool Game2048::moveTiles(char direction) {
    bool moved = false;

    for (int i = 0; i < SIZE; ++i) {
        for (int j = 0; j < SIZE; ++j) {
            int row = i;
            int col = j;

            switch (direction) {
                case 'W': // Up
                    row = i - 1;
                    break;
                case 'S': // Down
                    row = i + 1;
                    break;
                case 'A': // Left
                    col = j - 1;
                    break;
                case 'D': // Right
                    col = j + 1;
                    break;
                default:
                    break;
            }

            if (row >= 0 && row < SIZE && col >= 0 && col < SIZE && board[i][j] != 0) {
                // Move tile to the empty position
                while (board[row][col] == 0 && row >= 0 && row < SIZE && col >= 0 && col < SIZE) {
                    board[row][col] = board[row - (direction == 'S') + (direction == 'W')][col - (direction == 'D') + (direction == 'A')];
                    board[row - (direction == 'S') + (direction == 'W')][col - (direction == 'D') + (direction == 'A')] = 0;
                    row -= (direction == 'S') - (direction == 'W');
                    col -= (direction == 'D') - (direction == 'A');
                    moved = true;
                }
            }
        }
    }

    return moved;
}

bool Game2048::mergeTiles(char direction) {
    bool merged = false;

    for (int i = 0; i < SIZE; ++i) {
        for (int j = 0; j < SIZE; ++j) {
            int row = i;
            int col = j;

            switch (direction) {
                case 'W': // Up
                    row = i - 1;
                    break;
                case 'S': // Down
                    row = i + 1;
                    break;
                case 'A': // Left
                    col = j - 1;
                    break;
                case 'D': // Right
                    col = j + 1;
                    break;
                default:
                    break;
            }

            if (row >= 0 && row < SIZE && col >= 0 && col < SIZE && board[row][col] == board[i][j] && board[i][j] != 0) {
                // Merge tiles
                board[row][col] *= 2;
                board[i][j] = 0;
                merged = true;
            }
        }
    }

    return merged;
}

bool Game2048::isGameOver() {
    for (int i = 0; i < SIZE; ++i) {
        for (int j = 0; j < SIZE; ++j) {
            if (board[i][j] == 0) {
                return false; // Game is not over, there is an empty tile
            }

            // Check if there are adjacent tiles with the same value
            if ((i < SIZE - 1 && board[i][j] == board[i + 1][j]) ||
                (j < SIZE - 1 && board[i][j] == board[i][j + 1])) {
                return false; // Game is not over, there are still possible moves
            }
        }
    }

    return true; // Game over, no empty tile and no possible moves
}

void Game2048::play() {
    char move;

    while (!gameOver) {
        printBoard();

        cout << "Enter move (W/A/S/D): ";
        cin >> move;

        move = toupper(move);

        if (move == 'W' || move == 'A' || move == 'S' || move == 'D') {
            // Move tiles
            bool moved = moveTiles(move);

            // Merge tiles
            bool merged = mergeTiles(move);

            // Move tiles again after merging
            moveTiles(move);

            if (moved || merged) {
                generateRandomTile();
            }

            // Check if the game is over
            gameOver = isGameOver();
        } else {
            cout << "Invalid move! Please enter W, A, S, or D." << endl;
        }
    }

    // Game over
    printBoard();
    cout << "Game Over! Your score: ";

    int maxTile = 0;
    for (int i = 0; i < SIZE; ++i) {
        for (int j = 0; j < SIZE; ++j) {
            if (board[i][j] > maxTile) {
                maxTile = board[i][j];
            }
        }
    }

    cout << maxTile << endl;
}

void Game2048::delay(int milliseconds) {
    this_thread::sleep_for(chrono::milliseconds(milliseconds));
}

int main() {
    srand(static_cast<unsigned>(time(nullptr)));

    Game2048 game;
    game.play();

    return 0;
}
