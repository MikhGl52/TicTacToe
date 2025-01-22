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
    int winner = CheckWin(false);
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

int alphaBetta(Tcell field[numRows][numCols], int depth, bool isMaximizing, int alpha, int beta) {

    int score = evaluate(field);

    // Если кто-то победил, возвращаем оценку
    if (score != 0)
        return score;

    // Проверяем, остались ли свободные ходы
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

    // Максимизирующий игрок (крестики)
    if (isMaximizing) {
        int best = INT_MIN;

        for (int i = 0; i < numRows; i++) {
            for (int j = 0; j < numCols; j++) {
                if (field[i][j].symbol == empty) {
                    field[i][j].symbol = cross;
                    int value = alphaBetta(field, depth + 1, false, alpha, beta);
                    best = max(best, value);
                    alpha = max(alpha, best);
                    field[i][j].symbol = empty;

                    // Альфа-бета отсечение
                    if (beta <= alpha)
                        return best;
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
                    field[i][j].symbol = circle;
                    int value = alphaBetta(field, depth + 1, true, alpha, beta);
                    best = min(best, value);
                    beta = min(beta, best);
                    field[i][j].symbol = empty;

                    // Альфа-бета отсечение
                    if (beta <= alpha)
                        return best;
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

                field[i][j].symbol = currentPlayer;

                int moveValue = alphaBetta(field, 0, currentPlayer == circle, INT_MIN, INT_MAX);


                field[i][j].symbol = empty;


                if ((currentPlayer == cross && moveValue >= bestValue) ||
                    (currentPlayer == circle && moveValue <= bestValue)) {
                    if (moveValue == bestValue) {
                        if (rand() % 2 == 0) {
                            *bestRow = i;
                            *bestCol = j;
                        }
                    }
                    else {
                        bestValue = moveValue;
                        *bestRow = i;
                        *bestCol = j;
                    }

                }
            }
        }
    }
}