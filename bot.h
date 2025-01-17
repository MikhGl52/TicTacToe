#pragma once
#ifndef BOT_H
#define BOT_H
#include <GLFW/glfw3.h>
#include <windows.h>
#include <stdbool.h>
#include <time.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>  
#include <GL\freeglut.h>
#include "main.h"

void findBestMove(Tcell field[numRows][numCols], int* bestRow, int* bestCol, int currentPlayer);

#endif // BOT_H
