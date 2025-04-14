#define _CRTDBG_MAP_ALLOC
#ifdef _DEBUG
#ifndef DBG_NEW
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
#define new DBG_NEW
#endif // !DBG_NEW
#endif

#include <iostream>
#include <stdlib.h>
#include <crtdbg.h>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <windows.h>

using namespace std;

void initGame(int**& board, int& width, int& height);
void shuffleCard(int**& board, int width, int height);
void playCardMatchingGame(int** board, int width, int height);
void printBoard(int** board, bool** opened, int width, int height, int x1, int y1, int x2, int y2);
void freeMemory(int**& memory, int width);

int main() {
    int** Board = nullptr;
    int width, height;

    initGame(Board, width, height);
    playCardMatchingGame(Board, width, height);
    cout << "\n축하합니다! 모든 카드를 맞췄습니다 \n";
    freeMemory(Board, width);

    _CrtDumpMemoryLeaks(); 
    return 0;
}

void initGame(int**& board, int& width, int& height) {
    srand((unsigned int)time(NULL));
    int cardCount;
    cout << "202411292 박준건" << endl;
    do {
        cout << "게임에 사용할 카드 개수를 입력해주세요 (짝수만 가능): ";
        cin >> cardCount;
        if (cardCount % 2 != 0) {
            cout << "짝수로 입력해주세요!" << endl;
        }
    } while (cardCount % 2 != 0);

    width = 1;
    height = cardCount;
    for (int i = 1; i <= sqrt(cardCount); ++i) {
        if (cardCount % i == 0) {
            int j = cardCount / i;
            if (abs(i - j) < abs(width - height)) {
                width = i;
                height = j;
            }
        }
    }

    board = new int* [width];
    for (int i = 0; i < width; i++) {
        board[i] = new int[height] {0};
    }

    // 카드 값 할당
    int value = 1;
    int count = 0;
    for (int i = 0; i < width; ++i) {
        for (int j = 0; j < height; ++j) {
            board[i][j] = value;
            count++;
            if (count == 2) {
                value++;
                count = 0;
            }
        }
    }

    shuffleCard(board, width, height);
}

void shuffleCard(int**& board, int width, int height) {
    int total = width * height;
    int* temp = new int[total];
    int index = 0;

    for (int i = 0; i < width; ++i)
        for (int j = 0; j < height; ++j)
            temp[index++] = board[i][j];

    for (int i = total - 1; i > 0; --i) {
        int j = rand() % (i + 1);
        int tempVal = temp[i];
        temp[i] = temp[j];
        temp[j] = tempVal;
    }

    index = 0;
    for (int i = 0; i < width; ++i)
        for (int j = 0; j < height; ++j)
            board[i][j] = temp[index++];

    delete[] temp;
}

void playCardMatchingGame(int** board, int width, int height) {
    bool** opened = new bool* [width];
    for (int i = 0; i < width; ++i)
        opened[i] = new bool[height] {false};

    int matchedPairs = 0;
    int totalPairs = (width * height) / 2;

    while (matchedPairs < totalPairs) {
        printBoard(board, opened, width, height, -1, -1, -1, -1);

        int x1, y1, x2, y2;
        cout << "\n첫 번째 카드 좌표 입력 (행 열): ";
        cin >> x1 >> y1;
        cout << "두 번째 카드 좌표 입력 (행 열): ";
        cin >> x2 >> y2;

        if (x1 < 0 || x1 >= width || y1 < 0 || y1 >= height ||
            x2 < 0 || x2 >= width || y2 < 0 || y2 >= height) {
            cout << "보드 범위를 벗어났습니다. 다시 입력해주세요.\n";
            continue;
        }

        if (opened[x1][y1] || opened[x2][y2]) {
            cout << "이미 맞춘 카드입니다. 다시 선택해주세요.\n";
            continue;
        }
        if (x1 == x2 && y1 == y2) {
            cout << "중복된 입력입니다. 다시 입력해주세요\n";
            continue;
        }

        system("cls");
        cout << "[선택한 카드]\n";
        printBoard(board, opened, width, height, x1, y1, x2, y2);

        if (board[x1][y1] == board[x2][y2]) {
            cout << "\n짝을 맞췄습니다!\n";
            opened[x1][y1] = true;
            opened[x2][y2] = true;
            matchedPairs++;
        }
        else {
            cout << "\n틀렸습니다. 카드를 다시 숨깁니다...\n";
        }

        Sleep(2000);
        system("cls");
    }

    // 메모리 해제
    for (int i = 0; i < width; ++i)
        delete[] opened[i];
    delete[] opened;
}

void printBoard(int** board, bool** opened, int width, int height, int x1, int y1, int x2, int y2) {
    cout << "\n[현재 보드 상태]\n";
    for (int i = 0; i < width; ++i) {
        for (int j = 0; j < height; ++j) {
            if (opened[i][j] || (i == x1 && j == y1) || (i == x2 && j == y2))
                cout << board[i][j] << "\t";
            else
                cout << "*\t";
        }
        cout << endl;
    }
}

void freeMemory(int**& memory, int width) {
    for (int i = 0; i < width; ++i)
        delete[] memory[i];
    delete[] memory;
    memory = nullptr;
}
