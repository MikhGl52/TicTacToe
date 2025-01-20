#pragma once
#ifndef MAIN_H
#define MAIN_H

#include <GLFW/glfw3.h>
#include <windows.h>
#include <stdbool.h>
#include <time.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <GL/freeglut.h>

#pragma warning(disable : 4996) //_CRT_SECURE_NO_WARNINGS

#define numRows 3 // W
#define numCols 3 // H
#define M_PI 3.14159265358979323846  /* pi */

typedef enum {
    empty,
    cross,
    circle
} Symbol;

typedef struct {
    int symbol; // 0 - empty, 1 - cross, 2 - circle
} Tcell;

// Global variables
extern int windowWidth;
extern int windowHeight;
extern float cellSize;
extern int player;
extern int circleAmount;
extern int crossAmount;
extern int cellAmount;
extern int ended;
extern Tcell field[numRows][numCols];

// Function declarations
void showSymbol(int symbol);
void ShowField();
int CheckWin(bool flag);
void ScreenToOpenGL(HWND hwnd, int x, int y, float* ox, float* oy);
int InMap(int x, int y);
void NewGame();
void ShowGame();
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
void EnableOpenGL(HWND hwnd, HDC* hDC, HGLRC* hRC);
void DisableOpenGL(HWND hwnd, HDC hDC, HGLRC hRC);

#endif // MAIN_H
