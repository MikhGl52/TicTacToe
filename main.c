#include <GLFW/glfw3.h>
#include <windows.h>
#include <stdbool.h>
#include <time.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>  
#include <GL\freeglut.h>
#include "bot.h"


#pragma warning(disable : 4996) //_CRT_SECURE_NO_WARNINGS



int windowWidth = 1000;
int windowHeight = 1000;
float cellSize = 1.0f / numRows;


int player = cross;

int human = circle;
int bot = cross;

int circleAmount;
int crossAmount;
int cellAmount;
int ended = 0;
Tcell field[numRows][numCols];

void showSymbol(int symbol) {
    glLineWidth(5);

    if (symbol == cross) {
        glBegin(GL_LINES);
        glColor3f(1, 0, 0); // Красный
        glVertex2f(0.1, 0.1);
        glVertex2f(0.9, 0.9);
        glVertex2f(0.9, 0.1);
        glVertex2f(0.1, 0.9);
        glEnd();
    }

    if (symbol == circle) {
        glColor3f(0, 1, 0); // Зеленый
        glBegin(GL_LINE_LOOP);
        float radius = 0.4f; // Радиус окружности
        for (int i = 0; i < 360; i++) {
            float theta = i * M_PI / 180;
            glVertex2f(0.5f + radius * cos(theta), 0.5f + radius * sin(theta));
        }
        glEnd();
    }
}

void ShowField() {

    glLineWidth(1);
    glBegin(GL_TRIANGLE_STRIP);
    glColor3f(0.65, 0.65, 0.65);
    glVertex2f(0, 1);
    glColor3f(0.60, 0.60, 0.60);
    glVertex2f(0, 0);
    glVertex2f(1, 1);
    glColor3f(0.55, 0.55, 0.55);
    glVertex2f(1, 0);
    glEnd();

    glBegin(GL_LINE_LOOP);
    glColor3f(0, 0, 0);
    glVertex2f(0, 0);
    glVertex2f(0, 1);
    glVertex2f(1, 1);
    glVertex2f(1, 0);
    glEnd();
}

int CheckWin() {
    for (int i = 0; i < numRows; i++) {
        if (field[i][0].symbol == field[i][1].symbol && field[i][1].symbol == field[i][2].symbol && field[i][0].symbol != 0)
        {
            //ended = 1;
            return field[i][0].symbol; // Победа по строке
        }
    }
    for (int j = 0; j < numCols; j++) {
        if (field[0][j].symbol == field[1][j].symbol && field[1][j].symbol == field[2][j].symbol && field[0][j].symbol != 0)
        {
            //ended = 1;
            return field[0][j].symbol; // Победа по столбцу
        }
    }
    if (field[0][0].symbol == field[1][1].symbol && field[1][1].symbol == field[2][2].symbol && field[0][0].symbol != 0)
    {
        //ended = 1;
        return field[0][0].symbol; // Победа по диагонали
    }
    if (field[0][2].symbol == field[1][1].symbol && field[1][1].symbol == field[2][0].symbol && field[0][2].symbol != 0)
    {
        //ended = 1;
        return field[0][2].symbol; // Победа по диагонали
    }
 
    return 0; // Победителя нет
}

void ScreenToOpenGL(HWND hwnd, int x, int y, float* ox, float* oy) {
    RECT rct;
    GetClientRect(hwnd, &rct); // Клиентская область
    *ox = (x / (float)rct.right) * numCols;
    *oy = numRows - (y / (float)rct.bottom) * numRows;
}

// Проверка на то, что координаты находятся в пределах поля
int InMap(int x, int y) {
    return (x >= 0) && (y >= 0) && (x < numRows) && (y < numCols);
}

void NewGame() {
    ended = 0;
    circleAmount = 0;
    crossAmount = 0;
    cellAmount = 0;

    for (int i = 0; i < numRows; i++)
        for (int j = 0; j < numCols; j++) { 
            field[i][j].symbol = 0;
            cellAmount++;
        }
}



void ShowGame() {
    glLoadIdentity();
    glScalef(2.0 / numRows, 2.0 / numCols, 1);
    glTranslatef(-numRows * 0.5, -numCols * 0.5, 0);

    for (int i = 0; i < numRows; i++) {
        for (int j = 0; j < numCols; j++) {
            glPushMatrix();
            glTranslatef(j, i, 0);
            ShowField();
            if (field[i][j].symbol != empty)
                showSymbol(field[i][j].symbol);
            glPopMatrix();
        }

    }
    glLoadIdentity();

}


LRESULT CALLBACK WindowProc(HWND, UINT, WPARAM, LPARAM);
void EnableOpenGL(HWND hwnd, HDC*, HGLRC*);
void DisableOpenGL(HWND, HDC, HGLRC);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    srand(time(NULL)); // Инициализация генератора случайных чисел
    // Инициализация консоли
    AllocConsole();
    freopen("CONOUT$", "w", stdout);
    freopen("CONIN$", "r", stdin);

    // Initialize GLUT


    WNDCLASSEX wcex;
    HWND hwnd;
    HDC hDC;
    HGLRC hRC;
    MSG msg;
    BOOL bQuit = FALSE;

    /* register window class */
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_OWNDC;
    wcex.lpfnWndProc = WindowProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
    wcex.lpszMenuName = NULL;
    wcex.lpszClassName = "GLSample";
    wcex.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

    if (!RegisterClassEx(&wcex))
        return 0;

    /* create main window */
    hwnd = CreateWindowEx(
        0,                      // dwExStyle
        "GLSample",             // lpClassName
        "",                     // lpWindowName (строка заголовка окна)
        WS_OVERLAPPEDWINDOW,    // dwStyle
        CW_USEDEFAULT,          // x
        CW_USEDEFAULT,          // y
        windowWidth,            // nWidth
        windowHeight,           // nHeight
        NULL,                   // hWndParent
        NULL,                   // hMenu
        hInstance,              // hInstance
        NULL                    // lpParam
    );

    if (!hwnd)
        return 0;

    ShowWindow(hwnd, nCmdShow);

    /* enable OpenGL for the window */
    EnableOpenGL(hwnd, &hDC, &hRC);

    NewGame();
    

    /* program main loop */
    while (!bQuit) {
        /* check for messages */
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
            /* handle or dispatch messages */
            if (msg.message == WM_QUIT) {
                bQuit = TRUE;
            }
            else {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
        }
        else {
            /* OpenGL animation code goes here */

            glClearColor(0.375f, 0.375f, 0.375f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);



            glPushMatrix();
            glClear(GL_COLOR_BUFFER_BIT);

            ShowGame();

            glPopMatrix();
            SwapBuffers(hDC);
        }
    }

    /* shutdown OpenGL */
    DisableOpenGL(hwnd, hDC, hRC);

    /* destroy the window explicitly */
    DestroyWindow(hwnd);

    FreeConsole();

    return msg.wParam;
}



// Обработчик событий WindowProc
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
    case WM_CLOSE:
        PostQuitMessage(0);
        break;

    case WM_SIZE:
        windowWidth = LOWORD(lParam);
        windowHeight = HIWORD(lParam);
        glViewport(0, 0, windowWidth, windowHeight);
        glLoadIdentity();
        float k = windowWidth / (float)windowHeight;
        glOrtho(-k, k, -1, 1, -1, 1);
        break;



    case WM_LBUTTONDOWN: {
        POINTFLOAT pf;
        ScreenToOpenGL(hwnd, LOWORD(lParam), HIWORD(lParam), &pf.x, &pf.y);
        int x = (int)pf.x;
        int y = (int)pf.y;
        if (ended) {
            NewGame();
            break;
        }
        if (InMap(x, y)) {
            if (player == bot) {
                int bestRow, bestCol;

                findBestMove(field, &bestRow, &bestCol, bot);
                field[bestRow][bestCol].symbol = bot;
                player = human;
            }
            else {
                if (field[y][x].symbol == empty && ended == 0) {
                    field[y][x].symbol = player;
                    cellAmount--;
                    if (CheckWin() > 0) {
                        const char* playerName = (player == cross) ? "Cross" : "Circle";
                        printf("%s won the game!\n", playerName);
                    }
                    else {
                        if (cellAmount == 0) {
                            printf("DRAW\n");
                            //ended = 1;
                        }
                    }
                    player = (player == cross) ? circle : cross;
                    if (player == bot) {
                        int bestRow, bestCol;

                        findBestMove(field, &bestRow, &bestCol, bot);
                        printf("%d %d\n", bestRow, bestCol);
                        field[bestRow][bestCol].symbol = bot;
                        player = human;
                    }

                }
            }
        }


    } break;

    case WM_RBUTTONDOWN: {
        POINTFLOAT pf;
        ScreenToOpenGL(hwnd, LOWORD(lParam), HIWORD(lParam), &pf.x, &pf.y);
        int x = (int)pf.x;
        int y = (int)pf.y;
        NewGame();
    }
                       break;


    case WM_DESTROY:
        PostQuitMessage(0);
        break;

    case WM_KEYDOWN:
        if (wParam == VK_ESCAPE) {
            PostQuitMessage(0);
        }
        break;

    default:
        return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
    return 0;
}



void EnableOpenGL(HWND hwnd, HDC* hDC, HGLRC* hRC) {
    PIXELFORMATDESCRIPTOR pfd;
    int iFormat;

    /* get the device context (DC) */
    *hDC = GetDC(hwnd);

    /* set the pixel format for the DC */
    ZeroMemory(&pfd, sizeof(pfd));
    pfd.nSize = sizeof(pfd);
    pfd.nVersion = 1;
    pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
    pfd.iPixelType = PFD_TYPE_RGBA;
    pfd.cColorBits = 24;
    pfd.cDepthBits = 16;
    pfd.iLayerType = PFD_MAIN_PLANE;

    iFormat = ChoosePixelFormat(*hDC, &pfd);
    SetPixelFormat(*hDC, iFormat, &pfd);

    /* create and enable the render context (RC) */
    *hRC = wglCreateContext(*hDC);
    wglMakeCurrent(*hDC, *hRC);
}

void DisableOpenGL(HWND hwnd, HDC hDC, HGLRC hRC) {
    wglMakeCurrent(NULL, NULL);
    wglDeleteContext(hRC);
    ReleaseDC(hwnd, hDC);
}