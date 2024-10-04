#include<iostream>
#include<iomanip>
using namespace std;

const int SIZE = 10;
char playerBoard[SIZE][SIZE];
char enemyBoard[SIZE][SIZE];
bool hitBoard[SIZE][SIZE] = { false };
const int shipLengths[] = { 4, 3, 3, 2, 2, 2, 1, 1, 1, 1 };


void initializeBoard(char board[SIZE][SIZE]) {
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            board[i][j] = '.';
        }
    }
}


bool canPlaceShip(char board[SIZE][SIZE], int x, int y, int length, bool horizontal) {
    for (int i = 0; i < length; i++) {
        int checkX = horizontal ? x : x + i;
        int checkY = horizontal ? y : y + i;

        if (checkX < 0 || checkX >= SIZE || checkY < 0 || checkY >= SIZE || board[checkX][checkY] != '.') {
            return false;
        }

        for (int dx = -1; dx <= 1; dx++) {
            for (int dy = -1; dy <= 1; dy++) {
                int neighborX = checkX + dx;
                int neighborY = checkY + dy;
                if (neighborX >= 0 && neighborX < SIZE && neighborY >= 0 && neighborY < SIZE) {
                    if (board[neighborX][neighborY] == 'X') {
                        return false;
                    }
                }
            }
        }
    }
    return true;
}

bool placeShip(char board[SIZE][SIZE], int length) {
    int x, y;
    bool horizontal = rand() % 2;

    while (true) {
        x = rand() % SIZE;
        y = rand() % SIZE;

        if (canPlaceShip(board, x, y, length, horizontal)) {
            break;
        }
        horizontal = !horizontal;
    }

    for (int i = 0; i < length; i++) {
        if (horizontal) {
            board[x][y + i] = 'X';
        }
        else {
            board[x + i][y] = 'X';
        }
    }

    return true;
}

void randomPlaceShips(char board[SIZE][SIZE]) {
    for (int length : shipLengths) {
        placeShip(board, length);
    }
}

void printBoards(char playerBoard[SIZE][SIZE], char enemyBoard[SIZE][SIZE]) {
    cout << "\033[34m" << "  Player Board:" << "\033[0m";
    cout << "\033[31m" << "\t\t\t\t\t   Enemy Board : " << "\033[0m" << endl;

    cout << endl;
    cout << "   ";
    for (int j = 1; j <= SIZE; j++) {
        cout << setw(4) << j;
    }
    cout << "         ";
    for (int j = 1; j <= SIZE; j++) {
        cout << setw(4) << j;
    }
    cout << endl;
    cout << endl;

    for (int i = 0; i < SIZE; i++) {
        cout << setw(3) << (i + 1);
        for (int j = 0; j < SIZE; j++) {
            if (playerBoard[i][j] == 'X') {
                cout << "\033[34m" << setw(4) << playerBoard[i][j] << "\033[0m";
            }
            else if (playerBoard[i][j] == 'x') {
                cout << "\033[32m" << setw(4) << playerBoard[i][j] << "\033[0m";
            }
            else if (playerBoard[i][j] == '-') {
                cout << "\033[31m" << setw(4) << playerBoard[i][j] << "\033[0m";
            }
            else {
                cout << setw(4) << playerBoard[i][j];
            }
        }
        cout << "      " << setw(3) << (i + 1);
        for (int j = 0; j < SIZE; j++) {
            if (enemyBoard[i][j] == 'X') {
                cout << setw(4) << '.';
            }
            else if (enemyBoard[i][j] == 'x') {
                cout << "\033[32m" << setw(4) << enemyBoard[i][j] << "\033[0m";
            }
            else if (enemyBoard[i][j] == '-') {
                cout << "\033[31m" << setw(4) << enemyBoard[i][j] << "\033[0m";
            }
            else {
                cout << setw(4) << enemyBoard[i][j];
            }
        }
        cout << endl;
        cout << endl;
    }
}


bool hit(char board[SIZE][SIZE], int x, int y) {
    if (board[x][y] == 'X') {
        board[x][y] = 'x';
        return true;
    }
    board[x][y] = '-';
    return false;
}

bool allShipsSunk(char board[SIZE][SIZE]) {
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            if (board[i][j] == 'X') {
                return false;
            }
        }
    }
    return true;
}

void main() {
    srand(time(NULL));

    initializeBoard(playerBoard);
    initializeBoard(enemyBoard);

    randomPlaceShips(playerBoard);
    randomPlaceShips(enemyBoard);

    cout << "Initial Boards:" << endl << endl;
    printBoards(playerBoard, enemyBoard);

    int x, y;
    bool playerTurn = true;
    int counter = 0;

    while (true) {
        counter++;

        if (playerTurn) {
            cout << "Your turn: " << endl;
            cout << "Enter row number(1-10): ";
            cin >> x;
            cout << "Enter column number(1-10): ";
            cin >> y;
            x--;
            y--;

            if (x < 0 || x >= SIZE || y < 0 || y >= SIZE) {
                cout << "Invalid coordinates. Try again.." << endl;
                counter--;
                continue;
            }

            if (hitBoard[x][y]) {
                cout << "This coordinate has already been entered. Enter another coordinate." << endl;
                counter--;
                continue;
            }

            hitBoard[x][y] = true;

            bool isHit = hit(enemyBoard, x, y);
            if (isHit) {
                cout << "Vurdun!" << endl;
            }
            else {
                cout << "Bomba!" << endl;
            }

            system("cls");
            cout << "Boards after player's turn:" << endl;
            printBoards(playerBoard, enemyBoard);

            if (allShipsSunk(enemyBoard)) {
                cout << "Congratulations, you won!" << endl;
                cout << "You won in " << counter << " turns." << endl;
                break;
            }

            playerTurn = isHit;
        }
        else {
            int enemyX, enemyY;
            enemyX = rand() % SIZE;
            enemyY = rand() % SIZE;

            while (playerBoard[enemyX][enemyY] == 'x' || playerBoard[enemyX][enemyY] == '-') {
                enemyX = rand() % SIZE;
                enemyY = rand() % SIZE;
            }

            cout << "The enemy is shooting: " << enemyX + 1 << " " << enemyY + 1 << endl;
            if (hit(playerBoard, enemyX, enemyY)) {
                cout << "The enemy took a hit!" << endl;
            }
            else {
                cout << "Bomba!" << endl;
                playerTurn = true;
            }

            system("cls");
            cout << "Enemy's turn: " << endl;
            cout << "Boards after enemy's turn:" << endl;
            printBoards(playerBoard, enemyBoard);

            if (allShipsSunk(playerBoard)) {
                cout << "The enemy won!" << endl;
                cout << "The enemy won in " << counter << " turns." << endl;
                break;
            }
        }
    }
}