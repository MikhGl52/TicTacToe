#include <GLFW/glfw3.h>
#include <windows.h>
#include <stdbool.h>
#include <time.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>  
#include <limits.h>
#include <GL\freeglut.h>
#include "main.h"

/*
crosses wins = 1
circles wins = -1
*/


int evaluate(Tcell field[numRows][numCols]) {
    int winner = CheckWin();
    if (winner == cross) return 1;  // Крестики выигрывают
    if (winner == circle) return -1;  // Нолики выигрывают
    return 0;  // Ничья
}


int minimax(Tcell field[numRows][numCols], int depth, bool isMaximizing) {

    int score = evaluate(field);

    if (score != 0)
        return score;

    bool movesLeft = false;
    for (int i = 0; i < numRows; i++) {
        for (int j = 0; j < numCols; j++) {
            if (field[i][j].symbol == empty) {
                movesLeft = true;
                break;
            }
        }
        if (movesLeft) break;
    }
    if (!movesLeft) return 0;  // Ничья


    if (isMaximizing) {//Максимизатор, крестик

        int best = INT_MIN;

        for (int i = 0; i < numRows; i++) {
            for (int j = 0; j < numCols; j++) {
                if (field[i][j].symbol == empty) {
                    field[i][j].symbol = cross;
                    best = max(best, minimax(field, depth + 1, false));
                    field[i][j].symbol = empty;
                }
            }
        }
        return best;
    }
    // Минимизирующий игрок (нолики)
    else {
        int best = INT_MAX;

        for (int i = 0; i < numRows; i++) {
            for (int j = 0; j < numCols; j++) {
                if (field[i][j].symbol == empty) {
                    field[i][j].symbol = circle;  // Пробуем ход
                    best = min(best, minimax(field, depth + 1, true));  // Рекурсивно вызываем для максимизирующего игрока
                    field[i][j].symbol = empty;  // Отменяем ход
                }
            }
        }
        return best;
    }
}

// Функция для нахождения лучшего хода
void findBestMove(Tcell field[numRows][numCols], int* bestRow, int* bestCol, int currentPlayer) {
    int bestValue = (currentPlayer == cross) ? INT_MIN : INT_MAX;
    //printf("%d", bestValue);
    *bestRow = -1;
    *bestCol = -1;

    for (int i = 0; i < numRows; i++) {
        for (int j = 0; j < numCols; j++) {
            if (field[i][j].symbol == empty) {
                // Сделать ход
                field[i][j].symbol = currentPlayer;

                // Оценить ход с помощью минимакса
                int moveValue = minimax(field, 0, currentPlayer == circle);

                // Отменить ход
                field[i][j].symbol = empty;

                // Обновить лучший ход
                if ((currentPlayer == cross && moveValue > bestValue) ||
                    (currentPlayer == circle && moveValue < bestValue)) {
                    bestValue = moveValue;
                    *bestRow = i;
                    *bestCol = j;
                }
            }
        }
    }
}