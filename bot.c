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
    if (winner == cross) return 1;  // �������� ����������
    if (winner == circle) return -1;  // ������ ����������
    return 0;  // �����
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
    if (!movesLeft) return 0;  // �����


    if (isMaximizing) {//������������, �������

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
    // �������������� ����� (������)
    else {
        int best = INT_MAX;

        for (int i = 0; i < numRows; i++) {
            for (int j = 0; j < numCols; j++) {
                if (field[i][j].symbol == empty) {
                    field[i][j].symbol = circle;  // ������� ���
                    best = min(best, minimax(field, depth + 1, true));  // ���������� �������� ��� ���������������� ������
                    field[i][j].symbol = empty;  // �������� ���
                }
            }
        }
        return best;
    }
}

// ������� ��� ���������� ������� ����
void findBestMove(Tcell field[numRows][numCols], int* bestRow, int* bestCol, int currentPlayer) {
    int bestValue = (currentPlayer == cross) ? INT_MIN : INT_MAX;
    //printf("%d", bestValue);
    *bestRow = -1;
    *bestCol = -1;

    for (int i = 0; i < numRows; i++) {
        for (int j = 0; j < numCols; j++) {
            if (field[i][j].symbol == empty) {
                // ������� ���
                field[i][j].symbol = currentPlayer;

                // ������� ��� � ������� ���������
                int moveValue = minimax(field, 0, currentPlayer == circle);

                // �������� ���
                field[i][j].symbol = empty;

                // �������� ������ ���
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