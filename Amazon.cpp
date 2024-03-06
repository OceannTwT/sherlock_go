#define _CRT_SECURE_NO_WARNINGS
#include "Amazon.h"
#include <conio.h>
#include <iostream>
#include <string>
using namespace std;

CTranspositionTable T;
CHistoryHeuristic C;
char a[10] = {65, 66, 67, 68, 69, 70, 71, 72, 73, 74};
int b[9] = {49, 50, 51, 52, 53, 54, 55, 56, 57};

chessboard::chessboard() {
    int i, j;
    for (i = 0; i < 10; i++) {
        for (j = 0; j < 10; j++) {
            B[i][j] = NOCHESS;
        }
    }
    B[0][3] = QueenB;
    B[0][6] = QueenB;
    B[3][0] = QueenB;
    B[3][9] = QueenB;
    B[9][3] = QueenW;
    B[9][6] = QueenW;
    B[6][0] = QueenW;
    B[6][9] = QueenW;
    //下方棋四个子的位置
    WBposition[0][0].from.x = 9;
    WBposition[0][0].from.y = 3;
    WBposition[0][1].from.x = 9;
    WBposition[0][1].from.y = 6;
    WBposition[0][2].from.x = 6;
    WBposition[0][2].from.y = 0;
    WBposition[0][3].from.x = 6;
    WBposition[0][3].from.y = 9;
    //上方棋四个子的位置
    WBposition[1][0].from.x = 0;
    WBposition[1][0].from.y = 3;
    WBposition[1][1].from.x = 0;
    WBposition[1][1].from.y = 6;
    WBposition[1][2].from.x = 3;
    WBposition[1][2].from.y = 0;
    WBposition[1][3].from.x = 3;
    WBposition[1][3].from.y = 9;

    gamer = QueenW;
    count = 0;

    DEPTH = 7; //控制亚马逊时间，最强7，用时30
    TIME = 20000;

    TimeCount = 0;
    timecount = 0;
    COUNT = 0;
    m_progress = 0;
    max_depth = 3;
    alltime = 0;
    steps = 1;
}

//绘制屏幕
void chessboard::StartScreen() {
    setbkcolor(WHITE);
    settextcolor(BLUE);
    cleardevice(); //// 用背景色清空屏幕
    IMAGE background;
    IMAGE start;
    IMAGE exit;
    IMAGE record;
    IMAGE confirm;
    loadimage(&background, _T("backg.jpg"));
    loadimage(&start, _T("start.jpg"));
    //	Resize(&start,114,65);
    loadimage(&record, _T("record.jpg"));
    loadimage(&exit, _T("exit2.jpg"));
    loadimage(&confirm, _T("confirm.jpg"));
    //批绘图模式先不显示
    BeginBatchDraw();
    //画图片
    putimage(0, 0, &background);
    putimage(550, 100, &start);
    putimage(550, 200, &record);
    putimage(550, 300, &exit);
    putimage(550, 400, &confirm);
    //画坐标
    int j = 0;
    for (int i = 20; i < 520; i = i + 50) {
        outtextxy(i, 505, a[j]);
        j++;
    }
    j = 0;
    for (int i = 20; i < 520; i = i + 50) {
        if (9 - j == 9) {
            outtextxy(505, i, 49); // 10
            outtextxy(510, i, 48);
        } else
            outtextxy(505, i, b[9 - j]);
        j++;
    }
    FlushBatchDraw(); //批量显示
    EndBatchDraw();
}

//绘制棋盘（黑子，白子，格子）
void chessboard::InitialQipan() {
    IMAGE white;
    IMAGE black;
    IMAGE hinder;
    IMAGE nochess;

    loadimage(&white, _T("CHESS_B.bmp"));
    loadimage(&hinder, _T("Hinder.jpg"));
    loadimage(&black, _T("CHESS_W.bmp"));
    loadimage(&nochess, _T("nochess.jpg"));
    if (mode_choose == 0 || mode_choose == 1) {
        for (int i = 0; i < 10; i++) {
            for (int j = 0; j < 10; j++) {
                switch (B[j][i]) {
                case HINDER:
                    putimage(i * 50, j * 50, &hinder);
                    break;
                case QueenW:
                    putimage(i * 50, j * 50, &white);
                    break;
                case QueenB:
                    putimage(i * 50, j * 50, &black);
                    break;
                case NOCHESS:
                    putimage(i * 50, j * 50, &nochess);
                    break;
                default:
                    break;
                }
            }
        }
    } else {
        for (int i = 0; i < 10; i++) {
            for (int j = 0; j < 10; j++) {
                switch (B[j][i]) {
                case HINDER:
                    putimage(i * 50, j * 50, &hinder);
                    break;
                case QueenW:
                    putimage(i * 50, j * 50, &black);
                    break;
                case QueenB:
                    putimage(i * 50, j * 50, &white);
                    break;
                case NOCHESS:
                    putimage(i * 50, j * 50, &nochess);
                    break;
                default:
                    break;
                }
            }
        }
    }
}

void chessboard::StartGame() {}

bool chessboard::ischesslegal(int x, int y, int z, int k, int gamer) {
    if (B[x][y] != gamer || B[z][k] != NOCHESS || x < 0 || x > 9 || y < 0 ||
        y > 9 || z < 0 || z > 9 || k < 0 || k > 9 || (x == z && y == k)) {
        return false;
    }
    int m = where(x, y, z, k);
    switch (m) {
    case 1:
        return leftup(x, y, z, k);
    case 2:
        return up(x, y, z, k);
    case 3:
        return rightup(x, y, z, k);
    case 4:
        return right(x, y, z, k);
    case 5:
        return rightdown(x, y, z, k);
    case 6:
        return down(x, y, z, k);
    case 7:
        return leftdown(x, y, z, k);
    case 8:
        return left(x, y, z, k);
    }
    return false;
}

bool chessboard::ishinderlegal(int x, int y) {
    if (B[x][y] != NOCHESS || x < 0 || x > 9 || y < 0 || y > 9) {
        return false;
    }
    int m = where(move.to.x, move.to.y, x, y);
    switch (m) {
    case 1:
        return leftup(move.to.x, move.to.y, x, y);
    case 2:
        return up(move.to.x, move.to.y, x, y);
    case 3:
        return rightup(move.to.x, move.to.y, x, y);
    case 4:
        return right(move.to.x, move.to.y, x, y);
    case 5:
        return rightdown(move.to.x, move.to.y, x, y);
    case 6:
        return down(move.to.x, move.to.y, x, y);
    case 7:
        return leftdown(move.to.x, move.to.y, x, y);
    case 8:
        return left(move.to.x, move.to.y, x, y);
    }
    return false;
}

int chessboard::creatmove(int gamer, int depth) {
    COUNT = 0;
    int i, j, k;
    chess m;
    if (gamer == -1) {
        i = 1;
    } else {
        i = 0;
    }
    for (j = 0; j < 4; j++) {
        int w = 1;
        m.from.x = WBposition[i][j].from.x;
        m.from.y = WBposition[i][j].from.y;
        m.to.x = WBposition[i][j].from.x;
        m.to.y = WBposition[i][j].from.y;
        while (w < 9) {
            switch (w) {
            case 1:
                while (m.to.x > 0 && m.to.y > 0) {
                    m.to.x--;
                    m.to.y--;
                    if (ischesslegal(m.from.x, m.from.y, m.to.x, m.to.y,
                                     gamer)) {
                        B[m.from.x][m.from.y] = NOCHESS;
                        B[m.to.x][m.to.y] = gamer;
                        hindermove(m, gamer, depth);
                        B[m.from.x][m.from.y] = gamer;
                        B[m.to.x][m.to.y] = NOCHESS;
                    } else {
                        break;
                    }
                }
                m.to.x = WBposition[i][j].from.x;
                m.to.y = WBposition[i][j].from.y;
                break;
            case 2:
                while (m.to.x > 0) {
                    m.to.x--;
                    if (ischesslegal(m.from.x, m.from.y, m.to.x, m.to.y,
                                     gamer)) {
                        B[m.from.x][m.from.y] = NOCHESS;
                        B[m.to.x][m.to.y] = gamer;
                        hindermove(m, gamer, depth);
                        B[m.from.x][m.from.y] = gamer;
                        B[m.to.x][m.to.y] = NOCHESS;
                    } else {
                        break;
                    }
                }
                m.to.x = WBposition[i][j].from.x;
                m.to.y = WBposition[i][j].from.y;
                break;
            case 3:
                while (m.to.x > 0 && m.to.y < 10) {
                    m.to.x--;
                    m.to.y++;
                    if (ischesslegal(m.from.x, m.from.y, m.to.x, m.to.y,
                                     gamer)) {
                        B[m.from.x][m.from.y] = NOCHESS;
                        B[m.to.x][m.to.y] = gamer;
                        hindermove(m, gamer, depth);
                        B[m.from.x][m.from.y] = gamer;
                        B[m.to.x][m.to.y] = NOCHESS;
                    } else {
                        break;
                    }
                }
                m.to.x = WBposition[i][j].from.x;
                m.to.y = WBposition[i][j].from.y;
                break;
            case 4:
                while (m.to.y < 10) {
                    m.to.y++;
                    if (ischesslegal(m.from.x, m.from.y, m.to.x, m.to.y,
                                     gamer)) {
                        B[m.from.x][m.from.y] = NOCHESS;
                        B[m.to.x][m.to.y] = gamer;
                        hindermove(m, gamer, depth);
                        B[m.from.x][m.from.y] = gamer;
                        B[m.to.x][m.to.y] = NOCHESS;
                    } else {
                        break;
                    }
                }
                m.to.x = WBposition[i][j].from.x;
                m.to.y = WBposition[i][j].from.y;
                break;
            case 5:
                while (m.to.x < 10 && m.to.y < 10) {
                    m.to.x++;
                    m.to.y++;
                    if (ischesslegal(m.from.x, m.from.y, m.to.x, m.to.y,
                                     gamer)) {
                        B[m.from.x][m.from.y] = NOCHESS;
                        B[m.to.x][m.to.y] = gamer;
                        hindermove(m, gamer, depth);
                        B[m.from.x][m.from.y] = gamer;
                        B[m.to.x][m.to.y] = NOCHESS;
                    } else {
                        break;
                    }
                }
                m.to.x = WBposition[i][j].from.x;
                m.to.y = WBposition[i][j].from.y;
                break;
            case 6:
                while (m.to.x < 10) {
                    m.to.x++;
                    if (ischesslegal(m.from.x, m.from.y, m.to.x, m.to.y,
                                     gamer)) {
                        B[m.from.x][m.from.y] = NOCHESS;
                        B[m.to.x][m.to.y] = gamer;
                        hindermove(m, gamer, depth);
                        B[m.from.x][m.from.y] = gamer;
                        B[m.to.x][m.to.y] = NOCHESS;
                    } else {
                        break;
                    }
                }
                m.to.x = WBposition[i][j].from.x;
                m.to.y = WBposition[i][j].from.y;
                break;
            case 7:
                while (m.to.x < 10 && m.to.y > 0) {
                    m.to.x++;
                    m.to.y--;
                    if (ischesslegal(m.from.x, m.from.y, m.to.x, m.to.y,
                                     gamer)) {
                        B[m.from.x][m.from.y] = NOCHESS;
                        B[m.to.x][m.to.y] = gamer;
                        hindermove(m, gamer, depth);
                        B[m.from.x][m.from.y] = gamer;
                        B[m.to.x][m.to.y] = NOCHESS;
                    } else {
                        break;
                    }
                }
                m.to.x = WBposition[i][j].from.x;
                m.to.y = WBposition[i][j].from.y;
                break;
            case 8:
                while (m.to.y > 0) {
                    m.to.y--;
                    if (ischesslegal(m.from.x, m.from.y, m.to.x, m.to.y,
                                     gamer)) {
                        B[m.from.x][m.from.y] = NOCHESS;
                        B[m.to.x][m.to.y] = gamer;
                        hindermove(m, gamer, depth);
                        B[m.from.x][m.from.y] = gamer;
                        B[m.to.x][m.to.y] = NOCHESS;
                    } else {
                        break;
                    }
                }
                m.to.x = WBposition[i][j].from.x;
                m.to.y = WBposition[i][j].from.y;
                break;
            }
            w++;
        }
    }
    return COUNT;
}

void chessboard::hindermove(chess m, int gamer, int depth) {
    m.hinderto.x = m.to.x;
    m.hinderto.y = m.to.y;
    int w1 = 1;
    while (w1 < 9) {
        switch (w1) {
        case 1:
            while (m.hinderto.x > 0 && m.hinderto.y > 0) {
                m.hinderto.x--;
                m.hinderto.y--;
                if (ischesslegal(m.to.x, m.to.y, m.hinderto.x, m.hinderto.y,
                                 gamer)) {
                    // road.push_back(m);
                    MoveList[depth][COUNT] = m;
                    COUNT++;
                } else {
                    break;
                }
            }
            m.hinderto.x = m.to.x;
            m.hinderto.y = m.to.y;
            break;
        case 2:
            while (m.hinderto.x > 0) {
                m.hinderto.x--;
                if (ischesslegal(m.to.x, m.to.y, m.hinderto.x, m.hinderto.y,
                                 gamer)) {
                    // road.push_back(m);
                    MoveList[depth][COUNT] = m;
                    COUNT++;
                } else {
                    break;
                }
            }
            m.hinderto.x = m.to.x;
            m.hinderto.y = m.to.y;
            break;
        case 3:
            while (m.hinderto.x > 0 && m.hinderto.y < 10) {
                m.hinderto.x--;
                m.hinderto.y++;
                if (ischesslegal(m.to.x, m.to.y, m.hinderto.x, m.hinderto.y,
                                 gamer)) {
                    // road.push_back(m);
                    MoveList[depth][COUNT] = m;
                    COUNT++;
                } else {
                    break;
                }
            }
            m.hinderto.x = m.to.x;
            m.hinderto.y = m.to.y;
            break;
        case 4:
            while (m.hinderto.y < 10) {
                m.hinderto.y++;
                if (ischesslegal(m.to.x, m.to.y, m.hinderto.x, m.hinderto.y,
                                 gamer)) {
                    // road.push_back(m);
                    MoveList[depth][COUNT] = m;
                    COUNT++;
                } else {
                    break;
                }
            }
            m.hinderto.x = m.to.x;
            m.hinderto.y = m.to.y;
            break;
        case 5:
            while (m.hinderto.x < 10 && m.hinderto.y < 10) {
                m.hinderto.x++;
                m.hinderto.y++;
                if (ischesslegal(m.to.x, m.to.y, m.hinderto.x, m.hinderto.y,
                                 gamer)) {
                    // road.push_back(m);
                    MoveList[depth][COUNT] = m;
                    COUNT++;
                } else {
                    break;
                }
            }
            m.hinderto.x = m.to.x;
            m.hinderto.y = m.to.y;
            break;
        case 6:
            while (m.hinderto.x < 10) {
                m.hinderto.x++;
                if (ischesslegal(m.to.x, m.to.y, m.hinderto.x, m.hinderto.y,
                                 gamer)) {
                    // road.push_back(m);
                    MoveList[depth][COUNT] = m;
                    COUNT++;
                } else {
                    break;
                }
            }
            m.hinderto.x = m.to.x;
            m.hinderto.y = m.to.y;
            break;
        case 7:
            while (m.hinderto.x < 10 && m.hinderto.y > 0) {
                m.hinderto.x++;
                m.hinderto.y--;
                if (ischesslegal(m.to.x, m.to.y, m.hinderto.x, m.hinderto.y,
                                 gamer)) {
                    // road.push_back(m);
                    MoveList[depth][COUNT] = m;
                    COUNT++;
                } else {
                    break;
                }
            }
            m.hinderto.x = m.to.x;
            m.hinderto.y = m.to.y;
            break;
        case 8:
            while (m.hinderto.y > 0) {
                m.hinderto.y--;
                if (ischesslegal(m.to.x, m.to.y, m.hinderto.x, m.hinderto.y,
                                 gamer)) {
                    // road.push_back(m);
                    MoveList[depth][COUNT] = m;
                    COUNT++;
                } else {
                    break;
                }
            }
            m.hinderto.x = m.to.x;
            m.hinderto.y = m.to.y;
            break;
        }
        w1++;
    }
}

//落子
void chessboard::domove(chess m) {
    B[m.to.x][m.to.y] = B[m.from.x][m.from.y]; //在新的位置放之前的棋子
    B[m.from.x][m.from.y] = NOCHESS;           //清空之前的位置
    int i, j;
    for (i = 0; i < 2; i++) {
        for (j = 0; j < 4; j++) {
            //根据位置信息遍历判断 哪一方的哪一个子 修改子的位置记录
            if (WBposition[i][j].from.x == m.from.x &&
                WBposition[i][j].from.y == m.from.y) {
                WBposition[i][j].from.x = m.to.x;
                WBposition[i][j].from.y = m.to.y;
            }
        }
    }
}

void chessboard::unmove(chess m) {
    B[m.from.x][m.from.y] = B[m.to.x][m.to.y];
    B[m.to.x][m.to.y] = NOCHESS;
    int i, j;
    for (i = 0; i < 2; i++) {
        for (j = 0; j < 4; j++) {
            if (WBposition[i][j].from.x == m.to.x &&
                WBposition[i][j].from.y == m.to.y) {
                WBposition[i][j].from.x = m.from.x;
                WBposition[i][j].from.y = m.from.y;
            }
        }
    }
}

//落箭
void chessboard::DOMOVE(chess m) { B[m.hinderto.x][m.hinderto.y] = HINDER; }

void chessboard::UNMOVE(chess m) { B[m.hinderto.x][m.hinderto.y] = NOCHESS; }

void chessboard::output() {
    int i, j, r = 0;
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
                            BACKGROUND_GREEN); //蓝色边框
    //重定向标准输出
    freopen("process.txt", "a", stdout); //
    cout << "  A B C D E F G H I J   " << endl;
    for (i = 0; i < 10; i++) {
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
                                BACKGROUND_GREEN);
        if (i == 9) {

            cout << i << " ";
        } else {
            cout << i << " ";
        }
        for (j = 0; j < 10; j++) {
            if (r % 2 == 1) {
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
                                        BACKGROUND_RED | BACKGROUND_GREEN |
                                            BACKGROUND_BLUE);
            } else {
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
                                        BACKGROUND_GREEN);
            }

            if (B[i][j] == NOCHESS) {
                cout << "  ";
            }
            if (B[i][j] == QueenW) {
                if (r % 2 == 1) {
                    SetConsoleTextAttribute(
                        GetStdHandle(STD_OUTPUT_HANDLE),
                        FOREGROUND_INTENSITY | BACKGROUND_RED |
                            BACKGROUND_GREEN | BACKGROUND_BLUE |
                            FOREGROUND_RED | FOREGROUND_GREEN |
                            FOREGROUND_BLUE);
                } else {

                    SetConsoleTextAttribute(
                        GetStdHandle(STD_OUTPUT_HANDLE),
                        FOREGROUND_INTENSITY | BACKGROUND_GREEN |
                            FOREGROUND_RED | FOREGROUND_GREEN |
                            FOREGROUND_BLUE);
                }
                if (mode_choose == 3) {
                    cout << "B ";
                } else {
                    cout << "W ";
                }
            }
            if (B[i][j] == QueenB) {
                /* if(r%2==1)
                 {
                     SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),FOREGROUND_INTENSITY|
                 BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE|
                 FOREGROUND_RED);
                 }
                 else
                 {

                     SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),FOREGROUND_INTENSITY|FOREGROUND_RED);
                 }*/
                if (mode_choose == 3) {
                    cout << "W ";
                } else {
                    cout << "B ";
                }
            }
            if (B[i][j] == HINDER) {
                /* if(r%2==1)
                 {
                     SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),FOREGROUND_INTENSITY|
                 BACKGROUND_RED | BACKGROUND_GREEN |
                 BACKGROUND_BLUE|FOREGROUND_RED|FOREGROUND_BLUE);
                 }
                 else
                 {

                     SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),FOREGROUND_INTENSITY|FOREGROUND_RED|FOREGROUND_BLUE);
                 }*/
                cout << "█";
            }
            r++;
        }
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
                                BACKGROUND_GREEN);
        if (i == 9) {

            cout << " " << i;
        } else {
            cout << " " << i;
        }
        cout << endl;
        r++;
    }
    cout << "  A B C D E F G H I J   " << endl;
    cout << endl;

    freopen("CON", "a", stdout); //输出到控制台
    cout << " ";
    cout << endl;
}

void chessboard::OUTPUT() {
    int i, j, r = 0;
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
                            BACKGROUND_GREEN); //蓝色边框

    freopen("process.txt", "a", stdout); //输出到文件
    cout << "  A B C D E F G H I J   " << endl;
    for (i = 0; i < 10; i++) {
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
                                BACKGROUND_GREEN);
        if (i == 9) {

            cout << i << " ";
        } else {
            cout << i << " ";
        }
        for (j = 0; j < 10; j++) {
            if (r % 2 == 1) {
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
                                        BACKGROUND_RED | BACKGROUND_GREEN |
                                            BACKGROUND_BLUE);
            } else {
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
                                        BACKGROUND_GREEN);
            }

            if (B[i][j] == NOCHESS) {
                cout << "  ";
            }
            if (B[i][j] == QueenB) {
                if (r % 2 == 1) {
                    SetConsoleTextAttribute(
                        GetStdHandle(STD_OUTPUT_HANDLE),
                        FOREGROUND_INTENSITY | BACKGROUND_RED |
                            BACKGROUND_GREEN | BACKGROUND_BLUE |
                            FOREGROUND_RED | FOREGROUND_GREEN |
                            FOREGROUND_BLUE);
                } else {

                    SetConsoleTextAttribute(
                        GetStdHandle(STD_OUTPUT_HANDLE),
                        FOREGROUND_INTENSITY | BACKGROUND_GREEN |
                            FOREGROUND_RED | FOREGROUND_GREEN |
                            FOREGROUND_BLUE);
                }
                if (mode_choose == 3) {
                    cout << "W ";
                } else {
                    cout << "B ";
                }
            }
            if (B[i][j] == QueenW) {
                /* if(r%2==1)
                 {
                     SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),FOREGROUND_INTENSITY|
                 BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE|
                 FOREGROUND_RED);
                 }
                 else
                 {

                     SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),FOREGROUND_INTENSITY|FOREGROUND_RED);
                 }*/

                if (mode_choose == 3) {
                    cout << "B ";
                } else {
                    cout << "W ";
                }
            }
            if (B[i][j] == HINDER) {
                /* if(r%2==1)
                 {
                     SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),FOREGROUND_INTENSITY|
                 BACKGROUND_RED | BACKGROUND_GREEN |
                 BACKGROUND_BLUE|FOREGROUND_RED|FOREGROUND_BLUE);
                 }
                 else
                 {

                     SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),FOREGROUND_INTENSITY|FOREGROUND_RED|FOREGROUND_BLUE);
                 }*/
                cout << "█";
            }
            r++;
        }
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
                                BACKGROUND_GREEN);
        if (i == 9) {

            cout << " " << i;
        } else {
            cout << " " << i;
        }
        cout << endl;
        r++;
    }
    cout << "  A B C D E F G H I J   " << endl;
    cout << endl;

    freopen("CON", "w", stdout); //输出到控制台

    // cout << "  A B C D E F G H I J   " << endl;
    // for (i = 0; i<10; i++)
    //{
    //	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
    //BACKGROUND_GREEN); 	if (i == 9)
    //	{

    //		cout << i << " ";
    //	}
    //	else
    //	{
    //		cout << i << " ";
    //	}
    //	for (j = 0; j<10; j++)
    //	{
    //		if (r % 2 == 1)
    //		{
    //			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
    //BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE);
    //		}
    //		else
    //		{
    //			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
    //BACKGROUND_GREEN);
    //		}

    //		if (B[i][j] == NOCHESS)
    //		{
    //			cout << "  ";
    //		}
    //		if (B[i][j] == QueenB)
    //		{
    //			if (r % 2 == 1)
    //			{
    //				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
    //FOREGROUND_INTENSITY | BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE
    //| FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
    //			}
    //			else
    //			{

    //				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
    //FOREGROUND_INTENSITY | BACKGROUND_GREEN | FOREGROUND_RED |
    //FOREGROUND_GREEN | FOREGROUND_BLUE);
    //			}
    //			cout << "●";
    //		}
    //		if (B[i][j] == QueenW)
    //		{
    //			/* if(r%2==1)
    //			{
    //			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),FOREGROUND_INTENSITY|
    //BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE| FOREGROUND_RED);
    //			}
    //			else
    //			{

    //			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),FOREGROUND_INTENSITY|FOREGROUND_RED);
    //			}*/

    //			cout << "●";
    //		}
    //		if (B[i][j] == HINDER)
    //		{
    //			/* if(r%2==1)
    //			{
    //			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),FOREGROUND_INTENSITY|
    //BACKGROUND_RED | BACKGROUND_GREEN |
    //BACKGROUND_BLUE|FOREGROUND_RED|FOREGROUND_BLUE);
    //			}
    //			else
    //			{

    //			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),FOREGROUND_INTENSITY|FOREGROUND_RED|FOREGROUND_BLUE);
    //			}*/
    //			cout << "█";
    //		}
    //		r++;
    //	}
    //	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
    //BACKGROUND_GREEN); 	if (i == 9)
    //	{

    //		cout << " " << i;
    //	}
    //	else
    //	{
    //		cout << " " << i;
    //	}
    //	cout << endl;
    //	r++;
    //}
    // cout << "  A B C D E F G H I J   " << endl;
    cout << " ";
    cout << endl;
}

bool chessboard::iswin() {
    int x, y, i;
    for (i = 0; i < 4; i++) {
        if (gamer == 1) {
            x = WBposition[0][i].from.x;
            y = WBposition[0][i].from.y;
        } else {
            x = WBposition[1][i].from.x;
            y = WBposition[1][i].from.y;
        }
        if (x > 0 && x < 9 && y > 0 && y < 9) {
            if (B[x - 1][y - 1] == 0 || B[x - 1][y] == 0 ||
                B[x - 1][y + 1] == 0 || B[x][y + 1] == 0 ||
                B[x + 1][y + 1] == 0 || B[x + 1][y] == 0 ||
                B[x + 1][y - 1] == 0 || B[x][y - 1] == 0) {
                return false;
            }
        }
        if (x == 0 && y == 0) {
            if (B[x][y + 1] == 0 || B[x + 1][y + 1] == 0 || B[x + 1][y] == 0) {
                return false;
            }
        }
        if (x == 0 && y == 9) {
            if (B[x + 1][y] == 0 || B[x + 1][y - 1] == 0 || B[x][y - 1] == 0) {
                return false;
            }
        }
        if (x == 9 && y == 0) {
            if (B[x - 1][y] == 0 || B[x - 1][y + 1] == 0 || B[x][y + 1] == 0) {
                return false;
            }
        }
        if (x == 9 && y == 9) {
            if (B[x - 1][y] == 0 || B[x][y - 1] == 0 || B[x - 1][y - 1] == 0) {
                return false;
            }
        }
        if (x == 0 && y < 9 && y > 0) {
            if (B[x][y + 1] == 0 || B[x + 1][y + 1] == 0 || B[x + 1][y] == 0 ||
                B[x + 1][y - 1] == 0 || B[x][y - 1] == 0) {
                return false;
            }
        }
        if (x == 9 && y < 9 && y > 0) {
            if (B[x][y + 1] == 0 || B[x - 1][y + 1] == 0 || B[x - 1][y] == 0 ||
                B[x - 1][y - 1] == 0 || B[x][y - 1] == 0) {
                return false;
            }
        }
        if (y == 0 && x < 9 && x > 0) {
            if (B[x - 1][y] == 0 || B[x - 1][y + 1] == 0 || B[x][y + 1] == 0 ||
                B[x + 1][y + 1] == 0 || B[x + 1][y] == 0) {
                return false;
            }
        }
        if (y == 9 && x < 9 && x > 0) {
            if (B[x - 1][y] == 0 || B[x - 1][y - 1] == 0 || B[x][y - 1] == 0 ||
                B[x + 1][y - 1] == 0 || B[x + 1][y] == 0) {
                return false;
            }
        }
    }
    return true;
}

int chessboard::where(int x, int y, int z, int k) {
    if (x - z > 0 && y - k > 0 && (x - z == y - k)) {
        return 1; //左上
    }
    if (x - z > 0 && y - k == 0) {
        return 2; //上
    }
    if (x - z > 0 && y - k < 0) {
        return 3; //右上
    }
    if (x - z == 0 && y - k < 0) {
        return 4; //右
    }
    if (x - z < 0 && y - k < 0 && (x - z == y - k)) {
        return 5; //右下
    }
    if (x - z < 0 && y - k == 0) {
        return 6; //下
    }
    if (x - z < 0 && y - k > 0) {
        return 7; //左下
    }
    if (x - z == 0 && y - k > 0) {
        return 8; //左
    }
}

bool chessboard::leftup(int x, int y, int z, int k) {
    while (x > 0 && y > 0) {
        x--;
        y--;
        if (B[x][y] != NOCHESS) {
            return false;
        }
        if (x == z && y == k) {
            return true;
        }
    }
    return false;
}

bool chessboard::up(int x, int y, int z, int k) {
    while (x > 0) {
        x--;
        if (B[x][y] != NOCHESS) {
            return false;
        }
        if (x == z && y == k) {
            return true;
        }
    }
    return false;
}

bool chessboard::rightup(int x, int y, int z, int k) {
    while (x > 0 && y < 10) {
        x--;
        y++;
        if (B[x][y] != NOCHESS) {
            return false;
        }
        if (x == z && y == k) {
            return true;
        }
    }
    return false;
}

bool chessboard::right(int x, int y, int z, int k) {
    while (y < 10) {
        y++;
        if (B[x][y] != NOCHESS) {
            return false;
        }
        if (x == z && y == k) {
            return true;
        }
    }
    return false;
}

bool chessboard::rightdown(int x, int y, int z, int k) {
    while (x < 10 && y < 10) {
        x++;
        y++;
        if (B[x][y] != NOCHESS) {
            return false;
        }
        if (x == z && y == k) {
            return true;
        }
    }
    return false;
}

bool chessboard::down(int x, int y, int z, int k) {
    while (x < 10) {
        x++;
        if (B[x][y] != NOCHESS) {
            return false;
        }
        if (x == z && y == k) {
            return true;
        }
    }
    return false;
}

bool chessboard::leftdown(int x, int y, int z, int k) {
    while (x < 10 && y > 0) {
        x++;
        y--;
        if (B[x][y] != NOCHESS) {
            return false;
        }
        if (x == z && y == k) {
            return true;
        }
    }
    return false;
}

bool chessboard::left(int x, int y, int z, int k) {
    while (y > 0) {
        y--;
        if (B[x][y] != NOCHESS) {
            return false;
        }
        if (x == z && y == k) {
            return true;
        }
    }
    return false;
}

void chessboard::CvsP() {
    freopen("log.txt", "w+", stdout);
    cout << "#[AM][先手][后手][后/先手胜][2018.08.03 14:00 合肥][2018 CCGC]"
         << endl;
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
                            FOREGROUND_INTENSITY | FOREGROUND_GREEN |
                                FOREGROUND_BLUE);
    int K = gamer;
    int d = 0;
    chess MOVE;
    IMAGE white;
    IMAGE black;
    IMAGE hinder;
    IMAGE nochess;
    loadimage(&white, _T("CHESS_B.bmp"));
    loadimage(&hinder, _T("Hinder.jpg"));
    loadimage(&black, _T("CHESS_W.bmp"));
    loadimage(&nochess, _T("nochess.jpg"));
    //我方先手 第一步指定位置
    if (gamer == 1) {
        move.from.x = 9;
        move.from.y = 3;
        move.to.x = 2;
        move.to.y = 3;
        move.hinderto.x = 4;
        move.hinderto.y = 1;
        domove(move);
        DOMOVE(move);
        if (K == 1)
            output(); //将标准输出重定向到proccess文件
        else
            OUTPUT();
        //重绘制棋盘
        InitialQipan();
        gamer = gamer * -1;
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
                                FOREGROUND_INTENSITY | FOREGROUND_GREEN |
                                    FOREGROUND_BLUE);
        freopen("process.txt", "a", stdout); //
        cout << "着棋:" << move.from.x << (char)(move.from.y + 'A') << "-"
             << move.to.x << (char)(move.to.y + 'A') << "(";
        cout << move.hinderto.x << (char)(move.hinderto.y + 'A') << ")" << endl;
        //控制台信息输出
        freopen("CON", "a", stdout); //
        cout << "着棋:" << move.from.x << (char)(move.from.y + 'A') << "-"
             << move.to.x << (char)(move.to.y + 'A') << "(";
        cout << move.hinderto.x << (char)(move.hinderto.y + 'A') << ")" << endl;
        cout << "遍历节点数:87564" << count << " "
             << "评估值:0"
             << " "
             << "本次运行时间:"
             << "0s "
             << "深度:" << max_depth << endl;
        steps = 0;
        cout << steps;
    }

    while (!iswin()) {
        if (steps == 0 && gamer == -1) {
            //棋谱保存
            freopen("log.txt", "a", stdout);
            cout << steps + 1 << ' ' << (char)(move.from.y + 'a')
                 << 10 - move.from.x << (char)(move.to.y + 'a')
                 << 10 - move.to.x << '(' << (char)(move.hinderto.y + 'a')
                 << 10 - move.hinderto.x << ')' << ' ';
            fclose(stdout);
            steps = 2;
        } else if (steps % 2 == 1) {
            freopen("log.txt", "a", stdout);
            cout << (steps + 1) / 2 << ' ';
        }
        if (gamer == 1) {
            T.CalculateInitHashKey(B);
            C.ResetHistoryTable();
            TimeCount = GetTickCount();
            timecount = TimeCount;
            m_progress++;
            // Alpha_beta剪枝
            // alpha_beta(-9999.0,9999.0,max_depth,gamer);
            if (behind == 2) {
                move.from.x = 3;
                move.from.y = 0;
                move.to.x = 8;
                move.to.y = 5;
                move.hinderto.x = 8;
                move.hinderto.y = 6;
                move.value = -0.9;
                behind = 0;
            } else if (behind == 4) {
                move.from.x = 3;
                move.from.y = 9;
                move.to.x = 8;
                move.to.y = 4;
                move.hinderto.x = 8;
                move.hinderto.y = 3;
                move.value = -0.9;
                behind = 0;
            }

            else {
                if (m_progress < 5) {
                    TIME = 30000;
                } else {
                    TIME = 35000;
                    /*DEPTH = 7;*/
                }

                for (max_depth = 1; max_depth <= DEPTH; max_depth++) {
                    if (psv_white(max_depth, -9999.0, 9999.0, gamer) != 666) {
                        MOVE = move;
                        d = max_depth;
                    }
                }
            }
            // move.value=psv_white(max_depth,-9999.0,9999.0,gamer);
            // m_progress++;
            timecount = GetTickCount() - timecount;
            alltime = alltime + timecount;
            domove(move);
            DOMOVE(move);
            if (K == 1)
                output();
            else
                OUTPUT();
            InitialQipan();

            // 设置画线颜色
            setlinecolor(RGB(255, 0, 0));
            // 画横线
            line(move.from.y * 50, move.from.x * 50 + 25, move.from.y * 50 + 50,
                 move.from.x * 50 + 25);
            // 画竖线
            line(move.from.y * 50 + 25, move.from.x * 50, move.from.y * 50 + 25,
                 move.from.x * 50 + 50);

            // 画横线
            line(move.to.y * 50, move.to.x * 50 + 25, move.to.y * 50 + 50,
                 move.to.x * 50 + 25);
            // 画竖线
            line(move.to.y * 50 + 25, move.to.x * 50, move.to.y * 50 + 25,
                 move.to.x * 50 + 50);

            // 画横线
            line(move.hinderto.y * 50, move.hinderto.x * 50 + 25,
                 move.hinderto.y * 50 + 50, move.hinderto.x * 50 + 25);
            // 画竖线
            line(move.hinderto.y * 50 + 25, move.hinderto.x * 50,
                 move.hinderto.y * 50 + 25, move.hinderto.x * 50 + 50);

            outtextxy(550, 50, 'O');
            outtextxy(560, 50, 'K');
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
                                    FOREGROUND_INTENSITY | FOREGROUND_GREEN |
                                        FOREGROUND_BLUE);
            freopen("process.txt", "a", stdout); //
            cout << "着棋:" << move.from.x << (char)(move.from.y + 'A') << "-"
                 << move.to.x << (char)(move.to.y + 'A') << "(";
            cout << move.hinderto.x << (char)(move.hinderto.y + 'A') << ")"
                 << endl;

            freopen("log.txt", "a", stdout);
            cout << (char)(move.from.y + 'a') << 10 - move.from.x
                 << (char)(move.to.y + 'a') << 10 - move.to.x << '('
                 << (char)(move.hinderto.y + 'a') << 10 - move.hinderto.x << ')'
                 << ' ';

            freopen("CON", "a", stdout); //
            cout << "着棋:" << move.from.x << (char)(move.from.y + 'A') << "-"
                 << move.to.x << (char)(move.to.y + 'A') << "(";
            cout << move.hinderto.x << (char)(move.hinderto.y + 'A') << ")"
                 << endl;
            cout << "遍历节点数:" << count << " "
                 << "评估值:" << move.value << " "
                 << "本次运行时间:" << timecount / 1000 << "s "
                 << "深度:" << d << endl;
            cout << "总用时:" << alltime / 60000 << "min" << endl;
            cout << "step" << m_progress << endl;
            cout << "depth" << DEPTH << endl;
            count = 0;

        } else {
            int time_push = 0;
            int hinder_push = 0;
            int p_x, p_y;
            MOUSEMSG p_m;
            string s;
            int x, y, z, k;
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
                                    FOREGROUND_INTENSITY | FOREGROUND_GREEN |
                                        FOREGROUND_BLUE);
            freopen("CON", "a", stdout); //
            cout << "选择要下的棋子" << endl;
            while (time_push % 2 != 0 || time_push == 0) {
                while (MouseHit()) // 当有鼠标消息的时候执行
                {
                    p_m = GetMouseMsg();
                    if (p_m.uMsg == WM_LBUTTONDOWN) //鼠标点击在某个按钮的范围
                    {
                        time_push++;
                        p_x = (int)(p_m.x - 5) / 50;
                        p_y = (int)(p_m.y - 5) / 50;
                        if (p_x >= 10 || p_y >= 10) {
                            time_push--;
                            break;
                        }
                        if (p_x >= 10 || p_y >= 10) {
                            time_push--;
                            break;
                        }
                        if (time_push == 1 && B[p_y][p_x] == gamer) {
                            s[0] = p_y;
                            s[1] = p_x;
                            putimage(p_x * 50, p_y * 50, &nochess);
                            cout << "选择" << p_x << "-" << p_y
                                 << endl; //调试用
                        } else if (time_push == 1 && B[p_y][p_x] != gamer) {
                            time_push--;
                        }

                        if (time_push == 2 && B[p_y][p_x] == NOCHESS) {
                            s[2] = p_y;
                            s[3] = p_x;
                            if (mode_choose == 1) {
                                putimage(p_x * 50, p_y * 50, &black);
                            } else if (mode_choose == 3) {
                                putimage(p_x * 50, p_y * 50, &white);
                            }
                            cout << "放在" << p_x << "-" << p_y
                                 << endl; //调试用
                        } else if (time_push == 2 && B[p_y][p_x] != NOCHESS) {
                            time_push--;
                        }
                    }
                }
            }
            time_push = 0;
            x = (int)(s[0]);
            y = (int)(s[1]);
            z = (int)(s[2]);
            k = (int)(s[3]);
            cout << "走棋" << x << (char)(y + 'A') << "-" << z
                 << (char)(k + 'A') << endl;
            move.from.x = x;
            move.from.y = y;
            move.to.x = z;
            move.to.y = k;
            while (!ischesslegal(move.from.x, move.from.y, move.to.x, move.to.y,
                                 gamer)) {
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
                                        FOREGROUND_INTENSITY |
                                            FOREGROUND_GREEN | FOREGROUND_BLUE);
                string map = "ABCDEFGHIJ";
                if (mode_choose == 1) {
                    putimage(move.from.y * 50, move.from.x * 50, &black);
                } else if (mode_choose == 3) {
                    putimage(move.from.y * 50, move.from.x * 50, &white);
                }
                putimage(move.to.y * 50, move.to.x * 50, &nochess);
                cout << "下棋错误请重新输入" << move.from.x << map[move.from.y]
                     << endl;
                while (time_push % 2 != 0 || time_push == 0) {
                    while (MouseHit()) // 当有鼠标消息的时候执行
                    {
                        p_m = GetMouseMsg();
                        if (p_m.uMsg ==
                            WM_LBUTTONDOWN) //鼠标点击在某个按钮的范围
                        {
                            time_push++;
                            p_x = (int)(p_m.x - 5) / 50;
                            p_y = (int)(p_m.y - 5) / 50;
                            if (p_x >= 10 || p_y >= 10) {
                                time_push--;
                                break;
                            }
                            if (time_push == 1 && B[p_y][p_x] == gamer) {
                                s[0] = p_y;
                                s[1] = p_x;
                                putimage(p_x * 50, p_y * 50, &nochess);
                                cout << "选择" << p_x << "-" << p_y
                                     << endl; //调试用
                            } else if (time_push == 1 && B[p_y][p_x] != gamer) {
                                time_push--;
                            }

                            if (time_push == 2 && B[p_y][p_x] == NOCHESS) {
                                s[2] = p_y;
                                s[3] = p_x;
                                if (mode_choose == 1) {
                                    putimage(p_x * 50, p_y * 50, &black);
                                } else if (mode_choose == 3) {
                                    putimage(p_x * 50, p_y * 50, &white);
                                }
                                cout << "放在" << p_x << "-" << p_y
                                     << endl; //调试用
                            } else if (time_push == 2 &&
                                       B[p_y][p_x] != NOCHESS) {
                                time_push--;
                            }
                        }
                    }
                }
                time_push = 0;
                x = (int)(s[0]);
                y = (int)(s[1]);
                z = (int)(s[2]);
                k = (int)(s[3]);
                cout << "走棋" << x << (char)(y + 'A') << "-" << z
                     << (char)(k + 'A') << endl;
                move.from.x = x;
                move.from.y = y;
                move.to.x = z;
                move.to.y = k;
            }
            domove(move);
            if (K == 1)
                output();
            else
                OUTPUT();
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
                                    FOREGROUND_INTENSITY | FOREGROUND_GREEN |
                                        FOREGROUND_BLUE);
            cout << "选择要下障碍位置" << endl;
            while (hinder_push == 0) {
                while (MouseHit()) // 当有鼠标消息的时候执行
                {
                    p_m = GetMouseMsg();
                    if (p_m.uMsg == WM_LBUTTONDOWN) //鼠标点击在某个按钮的范围
                    {
                        hinder_push++;
                        p_x = (int)(p_m.x - 5) / 50;
                        p_y = (int)(p_m.y - 5) / 50;
                        if (p_x >= 10 || p_y >= 10) {
                            hinder_push--;
                            break;
                        }
                        if (hinder_push == 1 && B[p_y][p_x] == NOCHESS) {
                            s[0] = p_y;
                            s[1] = p_x;
                            putimage(p_x * 50, p_y * 50, &hinder);
                            cout << "障碍放置在" << p_x << "-" << p_y
                                 << endl; //调试用
                        } else if (hinder_push == 1 && B[p_y][p_x] != NOCHESS) {
                            hinder_push--;
                        }
                    }
                }
            }
            hinder_push = 0;
            x = (int)(s[0]);
            y = (int)(s[1]);
            cout << "障碍" << x << (char)(y + 'A') << endl;
            move.hinderto.x = x;
            move.hinderto.y = y;
            while (!ishinderlegal(move.hinderto.x, move.hinderto.y)) {
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
                                        FOREGROUND_INTENSITY |
                                            FOREGROUND_GREEN | FOREGROUND_BLUE);
                putimage(move.hinderto.y * 50, move.hinderto.x * 50, &nochess);
                cout << "障碍位置错误请重新输入" << endl;
                while (hinder_push == 0) {
                    while (MouseHit()) // 当有鼠标消息的时候执行
                    {
                        p_m = GetMouseMsg();
                        if (p_m.uMsg ==
                            WM_LBUTTONDOWN) //鼠标点击在某个按钮的范围
                        {
                            hinder_push++;
                            p_x = (int)(p_m.x - 5) / 50;
                            p_y = (int)(p_m.y - 5) / 50;
                            if (p_x >= 10 || p_y >= 10) {
                                hinder_push--;
                                break;
                            }
                            if (hinder_push == 1 && B[p_y][p_x] == NOCHESS) {
                                s[0] = p_y;
                                s[1] = p_x;
                                putimage(p_x * 50, p_y * 50, &hinder);
                                cout << "障碍放置在" << p_x << "-" << p_y
                                     << endl; //调试用
                            } else if (hinder_push == 1 &&
                                       B[p_y][p_x] != NOCHESS) {
                                hinder_push--;
                            }
                        }
                    }
                }
                hinder_push = 0;
                x = (int)(s[0]);
                y = (int)(s[1]);
                cout << "障碍" << x << (char)(y + 'A') << endl;
                move.hinderto.x = x;
                move.hinderto.y = y;
            }
            DOMOVE(move);
            if (K == 1)
                output();
            else
                OUTPUT();
            InitialQipan();
            char BACK = 0;
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
                                    FOREGROUND_INTENSITY | FOREGROUND_GREEN |
                                        FOREGROUND_BLUE);
            cout << "1 继续  2 悔棋" << endl;
            while (BACK == 0) {
                while (MouseHit()) // 当有鼠标消息的时候执行
                {
                    // 获取一条鼠标消息
                    p_m = GetMouseMsg();
                    if (p_m.uMsg == WM_LBUTTONDOWN) //鼠标点击在某个按钮的范围
                    {
                        if (p_m.x >= 550 && p_m.x <= 665 && p_m.y >= 300 &&
                            p_m.y <= 367) //悔棋
                        {
                            BACK = '2';
                            cout << "悔棋一步" << endl;
                        }
                        if (p_m.x >= 550 && p_m.x <= 665 && p_m.y >= 400 &&
                            p_m.y <= 467) //确认
                        {
                            BACK = '1';
                            if (behind == 1) {
                                cout << "test" << move.from.x << move.from.y
                                     << move.to.x << move.to.y
                                     << move.hinderto.x << move.hinderto.y
                                     << endl;
                                if (move.from.x == 9 && move.from.y == 3 &&
                                    move.to.x == 3 && move.to.y == 3 &&
                                    move.hinderto.x == 3 &&
                                    move.hinderto.y == 6) {
                                    behind = 2;
                                }
                                if (move.from.x == 9 && move.from.y == 6 &&
                                    move.to.x == 3 && move.to.y == 6 &&
                                    move.hinderto.x == 3 &&
                                    move.hinderto.y == 3) {
                                    behind = 4;
                                }
                            }
                            freopen("process.txt", "a", stdout); //输出到文件
                            cout << "敌方着棋:" << move.from.x
                                 << (char)(move.from.y + 'A') << "-"
                                 << move.to.x << (char)(move.to.y + 'A') << "(";
                            cout << move.hinderto.x
                                 << (char)(move.hinderto.y + 'A') << ")"
                                 << endl;
                            freopen("CON", "a", stdout); //输出到屏幕
                            cout << "开始计算..." << endl;
                        }
                    }
                }
            }
            while (BACK != '1') {
                UNMOVE(move);
                unmove(move);

                if (K == 1)
                    output();
                else
                    OUTPUT();
                InitialQipan();
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
                                        FOREGROUND_INTENSITY |
                                            FOREGROUND_GREEN | FOREGROUND_BLUE);
                cout << "选择要下的棋子" << endl;
                while (time_push % 2 != 0 || time_push == 0) {
                    while (MouseHit()) // 当有鼠标消息的时候执行
                    {
                        p_m = GetMouseMsg();
                        if (p_m.uMsg ==
                            WM_LBUTTONDOWN) //鼠标点击在某个按钮的范围
                        {
                            time_push++;
                            p_x = (int)(p_m.x - 5) / 50;
                            p_y = (int)(p_m.y - 5) / 50;
                            if (p_x >= 10 || p_y >= 10) {
                                time_push--;
                                break;
                            }
                            if (time_push == 1 && B[p_y][p_x] == gamer) {
                                s[0] = p_y;
                                s[1] = p_x;
                                putimage(p_x * 50, p_y * 50, &nochess);
                                cout << "选择" << p_x << "-" << p_y
                                     << endl; //调试用
                            } else if (time_push == 1 && B[p_y][p_x] != gamer) {
                                time_push--;
                            }

                            if (time_push == 2 && B[p_y][p_x] == NOCHESS) {
                                s[2] = p_y;
                                s[3] = p_x;
                                if (mode_choose == 1) {
                                    putimage(p_x * 50, p_y * 50, &black);
                                } else if (mode_choose == 3) {
                                    putimage(p_x * 50, p_y * 50, &white);
                                }
                                cout << "放在" << p_x << "-" << p_y
                                     << endl; //调试用
                            } else if (time_push == 2 &&
                                       B[p_y][p_x] != NOCHESS) {
                                time_push--;
                            }
                        }
                    }
                }
                time_push = 0;
                x = (int)(s[0]);
                y = (int)(s[1]);
                z = (int)(s[2]);
                k = (int)(s[3]);
                cout << "走棋" << x << (char)(y + 'A') << "-" << z
                     << (char)(k + 'A') << endl;
                move.from.x = x;
                move.from.y = y;
                move.to.x = z;
                move.to.y = k;
                while (!ischesslegal(move.from.x, move.from.y, move.to.x,
                                     move.to.y, gamer)) {
                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
                                            FOREGROUND_INTENSITY |
                                                FOREGROUND_GREEN |
                                                FOREGROUND_BLUE);
                    if (mode_choose == 1) {
                        putimage(move.from.y * 50, move.from.x * 50, &black);
                    } else if (mode_choose == 3) {
                        putimage(move.from.y * 50, move.from.x * 50, &white);
                    }
                    putimage(move.to.y * 50, move.to.x * 50, &nochess);
                    cout << "下棋错误请重新输入" << endl;
                    while (time_push % 2 != 0 || time_push == 0) {
                        while (MouseHit()) // 当有鼠标消息的时候执行
                        {
                            p_m = GetMouseMsg();
                            if (p_m.uMsg ==
                                WM_LBUTTONDOWN) //鼠标点击在某个按钮的范围
                            {
                                time_push++;
                                p_x = (int)(p_m.x - 5) / 50;
                                p_y = (int)(p_m.y - 5) / 50;
                                if (p_x >= 10 || p_y >= 10) {
                                    time_push--;
                                    break;
                                }
                                if (time_push == 1 && B[p_y][p_x] == gamer) {
                                    s[0] = p_y;
                                    s[1] = p_x;
                                    cout << "选择" << p_x << "-" << p_y
                                         << endl; //调试用
                                    putimage(p_x * 50, p_y * 50, &nochess);
                                } else if (time_push == 1 &&
                                           B[p_y][p_x] != gamer) {
                                    time_push--;
                                }

                                if (time_push == 2 && B[p_y][p_x] == NOCHESS) {
                                    s[2] = p_y;
                                    s[3] = p_x;
                                    if (mode_choose == 1) {
                                        putimage(p_x * 50, p_y * 50, &black);
                                    } else if (mode_choose == 3) {
                                        putimage(p_x * 50, p_y * 50, &white);
                                    }
                                    cout << "放在" << p_x << "-" << p_y
                                         << endl; //调试用
                                } else if (time_push == 2 &&
                                           B[p_y][p_x] != NOCHESS) {
                                    time_push--;
                                }
                            }
                        }
                    }
                    time_push = 0;
                    x = (int)(s[0]);
                    y = (int)(s[1]);
                    z = (int)(s[2]);
                    k = (int)(s[3]);
                    cout << "走棋" << x << (char)(y + 'A') << "-" << z
                         << (char)(k + 'A') << endl;
                    move.from.x = x;
                    move.from.y = y;
                    move.to.x = z;
                    move.to.y = k;
                }
                domove(move);
                if (K == 1)
                    output();
                else
                    OUTPUT();
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
                                        FOREGROUND_INTENSITY |
                                            FOREGROUND_GREEN | FOREGROUND_BLUE);
                cout << "选择要下障碍位置" << endl;
                while (hinder_push == 0) {
                    while (MouseHit()) // 当有鼠标消息的时候执行
                    {
                        p_m = GetMouseMsg();
                        if (p_m.uMsg ==
                            WM_LBUTTONDOWN) //鼠标点击在某个按钮的范围
                        {
                            hinder_push++;
                            p_x = (int)(p_m.x - 5) / 50;
                            p_y = (int)(p_m.y - 5) / 50;
                            if (p_x >= 10 || p_y >= 10) {
                                hinder_push--;
                                break;
                            }
                            if (hinder_push == 1 && B[p_y][p_x] == NOCHESS) {
                                s[0] = p_y;
                                s[1] = p_x;
                                putimage(p_x * 50, p_y * 50, &hinder);
                                cout << "障碍放置在" << p_x << "-" << p_y
                                     << endl; //调试用
                            } else if (hinder_push == 1 &&
                                       B[p_y][p_x] != NOCHESS) {
                                hinder_push--;
                            }
                        }
                    }
                }
                hinder_push = 0;
                x = (int)(s[0]);
                y = (int)(s[1]);
                move.hinderto.x = x;
                move.hinderto.y = y;
                while (!ishinderlegal(move.hinderto.x, move.hinderto.y)) {
                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
                                            FOREGROUND_INTENSITY |
                                                FOREGROUND_GREEN |
                                                FOREGROUND_BLUE);
                    putimage(move.hinderto.y * 50, move.hinderto.x * 50,
                             &nochess);
                    cout << "障碍位置错误请重新输入" << endl;
                    while (hinder_push == 0) {
                        while (MouseHit()) // 当有鼠标消息的时候执行
                        {
                            p_m = GetMouseMsg();
                            if (p_m.uMsg ==
                                WM_LBUTTONDOWN) //鼠标点击在某个按钮的范围
                            {
                                hinder_push++;
                                p_x = (int)(p_m.x - 5) / 50;
                                p_y = (int)(p_m.y - 5) / 50;
                                if (p_x >= 10 || p_y >= 10) {
                                    hinder_push--;
                                    break;
                                }
                                if (hinder_push == 1 &&
                                    B[p_y][p_x] == NOCHESS) {
                                    s[0] = p_y;
                                    s[1] = p_x;
                                    putimage(p_x * 50, p_y * 50, &hinder);
                                    cout << "障碍放置在" << p_x << "-" << p_y
                                         << endl; //调试用
                                } else if (hinder_push == 1 &&
                                           B[p_y][p_x] != NOCHESS) {
                                    hinder_push--;
                                }
                            }
                        }
                    }
                    hinder_push = 0;
                    x = (int)(s[0]);
                    y = (int)(s[1]);
                    move.hinderto.x = x;
                    move.hinderto.y = y;
                }
                DOMOVE(move);
                if (K == 1)
                    output();
                else
                    OUTPUT();
                InitialQipan();
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
                                        FOREGROUND_INTENSITY |
                                            FOREGROUND_GREEN | FOREGROUND_BLUE);
                cout << "1 继续  2 悔棋" << endl;
                BACK = 0;
                while (BACK == 0) {
                    while (MouseHit()) // 当有鼠标消息的时候执行
                    {
                        // 获取一条鼠标消息
                        p_m = GetMouseMsg();
                        if (p_m.uMsg ==
                            WM_LBUTTONDOWN) //鼠标点击在某个按钮的范围
                        {
                            if (p_m.x >= 550 && p_m.x <= 665 && p_m.y >= 300 &&
                                p_m.y <= 367) //悔棋
                            {
                                BACK = '2';
                                cout << "悔棋一步" << endl;
                            }
                            if (p_m.x >= 550 && p_m.x <= 665 && p_m.y >= 400 &&
                                p_m.y <= 467) //确认
                            {
                                BACK = '1';
                                if (behind == 1) {
                                    cout << "test" << move.from.x << move.from.y
                                         << move.to.x << move.to.y
                                         << move.hinderto.x << move.hinderto.y
                                         << endl;
                                    if (move.from.x == 9 && move.from.y == 3 &&
                                        move.to.x == 3 && move.to.y == 3 &&
                                        move.hinderto.x == 3 &&
                                        move.hinderto.y == 6) {
                                        behind = 2;
                                    }
                                    if (move.from.x == 9 && move.from.y == 6 &&
                                        move.to.x == 3 && move.to.y == 6 &&
                                        move.hinderto.x == 3 &&
                                        move.hinderto.y == 3) {
                                        behind = 4;
                                    }
                                }
                                freopen("process.txt", "a",
                                        stdout); //输出到文件
                                cout << "敌方着棋:" << move.from.x
                                     << (char)(move.from.y + 'A') << "-"
                                     << move.to.x << (char)(move.to.y + 'A')
                                     << "(";
                                cout << move.hinderto.x
                                     << (char)(move.hinderto.y + 'A') << ")"
                                     << endl;
                                freopen("CON", "a", stdout); //输出到屏幕
                                cout << "开始计算..." << endl;
                            }
                        }
                    }
                }
            }
            freopen("log.txt", "a", stdout);
            cout << (char)(move.from.y + 'a') << 10 - move.from.x
                 << (char)(move.to.y + 'a') << 10 - move.to.x << '('
                 << (char)(move.hinderto.y + 'a') << 10 - move.hinderto.x << ')'
                 << ' ';
        }
        gamer = -gamer;
        steps++;
        if (steps % 2 == 1) {
            freopen("log.txt", "a", stdout);
            cout << endl;
        }
    }
    if (K == 1) {
        if (gamer == 1) {
            cout << "黑棋胜" << endl;
            freopen("a.txt", "w", stdout);
        } else {
            cout << "白棋胜" << endl;
            freopen("a.txt", "w", stdout);
        }
    } else {
        if (gamer == 1) {
            cout << "白棋胜" << endl;
            freopen("a.txt", "w", stdout);
        } else {
            cout << "黑棋胜" << endl;
            freopen("a.txt", "w", stdout);
        }
    }
}

void chessboard::PvsC() {
    output1();
    gamer = -1;
    CvsP();
}

int g_queen_direction[] = {-1, -13, -12, -11, 1, 13, 12, 11};

// 动态调整权值
void chessboard::Weight(double ww, double &w1, double &w2, double &w3,
                        double &w4) {
    if (ww >= 0 && ww <= 1) // 30-
        w1 = 1, w2 = 0, w3 = 0, w4 = 0;
    /*if(ww>1 && ww<=10)                          //24-27
            w1 = 0.6, w2 = 0, w3 = 0.2, w4 = 0.2;
    if (ww>10 && ww<=20)                          //20-24
            w1 = 0.5, w2 = 0, w3 = 0.3, w4 = 0.2;
    if (ww>20 && ww<=30)                          //20-24
            w1 = 0.4, w2 = 0.2, w3 = 0.2, w4 = 0.2;*/
    if (ww > 1 && ww <= 45) // 20-24
        w1 = 0.3, w2 = 0.3, w3 = 0.2, w4 = 0.2;
    if (ww > 45 && ww <= 55) // 15-20
        w1 = 0, w2 = 0.4, w3 = 0.3, w4 = 0.3;
    if (ww > 55 && ww <= 92) // 1
        w1 = 0, w2 = 0.3, w3 = 0.3, w4 = 0.4;
}

double chessboard::Evaluate() //评估函数
{
    int i, j, k; //循环变量
    // queen[]初始化
    for (i = 0; i < 4; i++) {
        queen[i] =
            (WBposition[0][i].from.x + 1) * 12 + WBposition[0][i].from.y + 1;
        queen[i + 4] =
            (WBposition[1][i].from.x + 1) * 12 + WBposition[1][i].from.y + 1;
    }
    // board初始化
    for (i = 0; i < 12; i++) {
        board[i] = 3, board[12 * i] = 3, board[143 - i] = 3,
        board[143 - 12 * i] = 3;
    }
    //初始化当前棋盘状态
    for (i = 1; i < 11; i++) {
        for (j = 1; j < 11; j++) {
            k = 12 * i + j;
            board[k] = B[i - 1][j - 1];
            if (board[k] == -1) {
                board[k] = 2;
            }
            if (board[k] == 2) {
                board[k] = 3;
            }
        }
    }

    /**************第二次修改*****************/
    memcpy(b_analyze[0], board, 144 * 4);
    memcpy(b_analyze[1], board, 144 * 4);
    memcpy(dist_analyze[0], board, 144 * 4);
    memcpy(dist_analyze[1], board, 144 * 4);
    /**************第二次修改*****************/

    // 计算双方的Queen King Move
    init_dist1(2);
    init_dist1(1);
    init_dist2(2);
    init_dist2(1);
    //计算估值
    double w1, w2, w3, w4;
    double t1 = 0, t2 = 0, c, c1 = 0, c2 = 0;
    ww = 0;

    for (k = 13; k < 131; k++) {
        if (board[k] == 0) {
            if (b_analyze[0][k] == 0)
                b_analyze[0][k] = -101;
            if (b_analyze[1][k] == 0)
                b_analyze[1][k] = -101;
            if (dist_analyze[0][k] == 0)
                dist_analyze[0][k] = -101;
            if (dist_analyze[1][k] == 0)
                dist_analyze[1][k] = -101;
            //求解ww
            if (b_analyze[0][k] == -101 || b_analyze[1][k] == -101)
                ww += 0;
            else
                ww += pow(2.0, (-1.0) * fabs((double)(b_analyze[0][k] -
                                                      b_analyze[1][k])));

            //求解t1,t2
            if (b_analyze[0][k] == b_analyze[1][k] && b_analyze[0][k] > -101 &&
                b_analyze[1][k] > -101) {
                t1 -= 0.1;
            } else if (b_analyze[0][k] > b_analyze[1][k]) {
                t1 += 1;
            } else if (b_analyze[0][k] < b_analyze[1][k]) {
                t1 += (-1);
            }

            if (dist_analyze[0][k] == dist_analyze[1][k] &&
                dist_analyze[0][k] > -101 && dist_analyze[1][k] > -101) {
                t2 -= 0.1;
            } else if (dist_analyze[0][k] > dist_analyze[1][k]) {
                t2 += 1;
            } else if (dist_analyze[0][k] < dist_analyze[1][k]) {
                t2 += (-1);
            }
            //求解c1,c2
            c = pow(2.0, 1.0 * (1 + b_analyze[0][k])) -
                pow(2.0, 1.0 * (1 + b_analyze[1][k]));
            c1 += c;
            c = (dist_analyze[0][k] - dist_analyze[1][k]) / 6.0;
            if (c > -1) {
                if (c > 1)
                    c = 1;
            } else
                c = -1;
            c2 += c;
        }
    }
    Weight(ww, w1, w2, w3, w4);
    return (w1 * t1 + w2 * t2 + w3 * c1 + w4 * c2); //返回估值
}

// QueenMove gengerate
void chessboard::init_dist1(int p) {
    /**************第二次修改*****************/
    int i1 = -2;
    int i3 = 4;
    int h1[144];
    h1[0] = queen[(p * 4 - 1)];
    h1[1] = queen[(p * 4 - 2)];
    h1[2] = queen[(p * 4 - 3)];
    h1[3] = queen[(p * 4 - 4)];
    /**************第二次修改*****************/
    do {
        int i;
        int h2[144]; /**************第二次修改*****************/
        for (int k = i = 0; k < i3; k++) {
            int i2 = h1[k];
            for (int j = 0; j < 8; j++) {
                int m = g_queen_direction[j];
                int n = i2 + m;
                do {
                    while (b_analyze[p - 1][n] == 0) {
                        b_analyze[p - 1][n] = i1;
                        h2[(i++)] = n;
                        n += m;
                    }
                    while (b_analyze[p - 1][n] == i1) {
                        n += m;
                    }
                } while (b_analyze[p - 1][n] ==
                         0); /**************第二次修改*****************/
            }
        }
        i3 = i;
        memcpy(h1, h2, 144 * 4); /**************第二次修改*****************/
        i1--;
    } while (i3 > 0);
}

// KingMove gengerate
void chessboard::init_dist2(int p) {
    /**************第二次修改*****************/
    int n = -2;
    int i2 = 4;
    int h1[144];
    h1[0] = queen[(p * 4 - 1)];
    h1[1] = queen[(p * 4 - 2)];
    h1[2] = queen[(p * 4 - 3)];
    h1[3] = queen[(p * 4 - 4)];
    /**************第二次修改*****************/
    do {
        int i;
        int h2[144];
        for (int k = i = 0; k < i2; k++) {
            int i1 = h1[k];
            for (int j = 0; j < 8; j++) {
                int m = i1 + g_queen_direction[j];
                if (dist_analyze[p - 1][m] == 0) {
                    dist_analyze[p - 1][m] = n;
                    h2[(i++)] = m;
                }
            }
        }
        i2 = i;
        memcpy(h1, h2, 144 * 4); /**************第二次修改*****************/
        n--;
    } while (i2 > 0);
}

float chessboard::alpha_beta(float alpha, float beta, int depth, int gamer) {
    int con;
    int i;
    int best = -1;
    float score;
    score = T.LookUpHashTable(alpha, beta, depth, (max_depth - depth) % 2);

    if (score != 66666.0) {
        return score;
    }

    if (depth <= 0) {
        // score=Value(road,gamer);
        if (gamer == -1)
            score = -Evaluate();
        else
            score = Evaluate();

        T.EnterHashTable(exact, score, depth, (max_depth - depth) % 2);
        return score;
    }
    con = creatmove(gamer, depth);
    if (depth == max_depth) {
        for (i = 0; i < con; i++) {
            //电脑走一步获得此走法的估值
            domove(MoveList[depth][i]);
            DOMOVE(MoveList[depth][i]);
            if (gamer == 1) {
                MoveList[depth][i].value = Evaluate();
            } else {
                MoveList[depth][i].value = -Evaluate();
            }
            UNMOVE(MoveList[depth][i]);
            unmove(MoveList[depth][i]);
        }
    } else {
        for (i = 0; i < con; i++) {
            MoveList[depth][i].value = C.GetHistoryScore(&MoveList[depth][i]);
        }
    }

    C.MergeSort(MoveList[depth], con, 0);
    int eval_is_exact = 0;
    int width;
    if (depth == max_depth) {
        if (m_progress <= 12)
            width = 120 + (m_progress - 1) * 20;
        else if (m_progress <= 18)
            width = 100 + (m_progress - 12) * 20;
        else
            width = 80 + (m_progress - 18) * 20;
        if (con <= width)
            width = con;
    } else
        width = con;
    for (i = 0; i < width; i++) {
        count++;
        domove(MoveList[depth][i]);
        DOMOVE(MoveList[depth][i]);
        T.Hash_MakeMove(MoveList[depth][i], gamer);
        // output();
        // Sleep(300);
        score = -alpha_beta(-beta, -alpha, depth - 1, -gamer);
        UNMOVE(MoveList[depth][i]);
        unmove(MoveList[depth][i]);
        T.Hash_UnMakeMove(MoveList[depth][i], gamer);
        // output();
        // Sleep(300);
        if (alpha < score) {
            alpha = score;
            eval_is_exact = 1;
            if (depth == max_depth) {
                move = MoveList[depth][i];
                move.value = score;
            }
            best = i;
        }
        if (alpha >= beta) {
            T.EnterHashTable(lowerbound, score, depth, (max_depth - depth) % 2);
            C.EnterHistoryScore(&MoveList[depth][i], depth);
            return alpha;
        }
    }
    if (eval_is_exact) {
        T.EnterHashTable(exact, alpha, depth, (max_depth - depth) % 2);
    } else {
        T.EnterHashTable(upperbound, alpha, depth, (max_depth - depth) % 2);
    }
    if (best != -1) {
        C.EnterHistoryScore(&MoveList[depth][i], depth);
    }
    return alpha;
}

float chessboard::psv_white(int depth, float alpha, float beta, int gamer) {
    int con;
    float score;
    float a, b, t;
    int best = -1;
    int i;
    /*if(iswin())
    {
        if(gamer==1)
        {
            return 6666.0;
        }
        else
        {
            return -6666.0;
        }
    }*/
    if (depth != max_depth) {
        score = T.LookUpHashTable(alpha, beta, depth, (max_depth - depth) % 2);
        if (score != 66666.0) {
            return score;
        }
    }
    if (depth <= 0) {
        if (gamer == -1) {
            score = -Evaluate();
        } else {
            score = Evaluate();
        }
        T.EnterHashTable(exact, score, depth, (max_depth - depth) % 2);
        return score;
    }
    // con=creatmove(gamer,depth);
    con = CREATEMOVE(depth, gamer);
    if (depth == max_depth) {
        for (i = 0; i < con; i++) {
            //电脑走一步获得此走法的估值
            domove(MoveList[depth][i]);
            DOMOVE(MoveList[depth][i]);
            if (gamer == 1) {
                MoveList[depth][i].value = Evaluate();
            } else {
                MoveList[depth][i].value = -Evaluate();
            }
            UNMOVE(MoveList[depth][i]);
            unmove(MoveList[depth][i]);
        }
    } else {
        for (i = 0; i < con; i++) {
            MoveList[depth][i].value = C.GetHistoryScore(&MoveList[depth][i]);
        }
    }
    C.MergeSort(MoveList[depth], con, 0);
    a = alpha;
    b = beta;
    int eval_is_exact = 0;
    if (depth == max_depth && max_depth > 1) {
        for (i = 0; i < con; i++) {
            if (MoveList[depth][i].from.x == move.from.x &&
                MoveList[depth][i].from.y == move.from.y &&
                MoveList[depth][i].to.x == move.to.x &&
                MoveList[depth][i].to.y == move.to.y &&
                MoveList[depth][i].hinderto.x == move.hinderto.x &&
                MoveList[depth][i].hinderto.y == move.hinderto.y) {
                MoveList[depth][i] = MoveList[depth][0];
                MoveList[depth][0] = move;
            }
        }
    }
    int width;
    if (depth == max_depth) {
        if (m_progress <= 12)
            width = 200; //+ (m_progress - 1) * 20;
        else if (m_progress <= 18)
            width = 200; //+ (m_progress - 12) * 20;
        else
            width = 200; //+ (m_progress - 18) * 20;
        if (con <= width)
            width = con;
    } else
        width = con;
    for (i = 0; i < width; i++) {
        if (GetTickCount() - TimeCount >= TIME) {
            return 666;
        }
        count++;
        T.Hash_MakeMove(MoveList[depth][i], gamer);
        domove(MoveList[depth][i]);
        DOMOVE(MoveList[depth][i]);
        t = -psv_white(depth - 1, -b, -a, -gamer);
        if (t > a && t < beta && i > 0) {
            // cout<<"*";
            a = -psv_white(depth - 1, -beta, -t, -gamer);
            eval_is_exact = 1;
            if (depth == max_depth) {
                move = MoveList[depth][i];
            }
            // BESTMOVE=i;
        }
        T.Hash_UnMakeMove(MoveList[depth][i], gamer);
        UNMOVE(MoveList[depth][i]);
        unmove(MoveList[depth][i]);
        if (a < t) {
            eval_is_exact = 1;
            a = t;
            if (depth == max_depth) {
                move = MoveList[depth][i];
                // move.value=a;
            }
            best = i;
        }
        if (a >= beta) {
            T.EnterHashTable(lowerbound, a, depth, (max_depth - depth) % 2);
            C.EnterHistoryScore(&MoveList[depth][i], depth);
            return a;
        }
        b = a + 1;
    }
    if (eval_is_exact)
        T.EnterHashTable(exact, a, depth, (max_depth - depth) % 2);
    else
        T.EnterHashTable(upperbound, a, depth, (max_depth - depth) % 2);
    if (best != -1) {
        C.EnterHistoryScore(&MoveList[depth][i], depth);
    }
    return a;
}

void chessboard::output1() {
    int i, j;
    for (i = 0; i < 10; i++) {
        for (j = 0; j < 10; j++) {
            B[i][j] = NOCHESS;
        }
    }
    B[0][3] = QueenW;
    B[0][6] = QueenW;
    B[3][0] = QueenW;
    B[3][9] = QueenW;
    B[9][3] = QueenB;
    B[9][6] = QueenB;
    B[6][0] = QueenB;
    B[6][9] = QueenB;
    WBposition[1][0].from.x = 9;
    WBposition[1][0].from.y = 3;
    WBposition[1][1].from.x = 9;
    WBposition[1][1].from.y = 6;
    WBposition[1][2].from.x = 6;
    WBposition[1][2].from.y = 0;
    WBposition[1][3].from.x = 6;
    WBposition[1][3].from.y = 9;
    WBposition[0][0].from.x = 0;
    WBposition[0][0].from.y = 3;
    WBposition[0][1].from.x = 0;
    WBposition[0][1].from.y = 6;
    WBposition[0][2].from.x = 3;
    WBposition[0][2].from.y = 0;
    WBposition[0][3].from.x = 3;
    WBposition[0][3].from.y = 9;
}

int chessboard::CREATEMOVE(int depth, int gamer) {
    COUNT = 0;
    int i;
    int fromx, fromy;
    if (gamer == 1) {
        for (i = 0; i < 4; i++) {
            fromx = WBposition[0][i].from.x;
            fromy = WBposition[0][i].from.y;
            B[fromx][fromy] = 0;
            CHESSMOVE(fromx, fromy, gamer, depth, i);
            B[fromx][fromy] = gamer;
        }
    } else {
        for (i = 0; i < 4; i++) {
            fromx = WBposition[1][i].from.x;
            fromy = WBposition[1][i].from.y;
            B[fromx][fromy] = 0;
            CHESSMOVE(fromx, fromy, gamer, depth, i);
            B[fromx][fromy] = gamer;
        }
    }
    return COUNT;
}

void chessboard::CHESSMOVE(int fromx, int fromy, int gamer, int depth, int i) {
    int x, y;
    x = fromx;
    y = fromy;
    while (fromx > 0 && fromy > 0) {
        fromx--;
        fromy--;
        if (B[fromx][fromy] == 0) {
            ARROWMOVE(x, y, fromx, fromy, gamer, depth, i);
        } else {
            fromx = x;
            fromy = y;
            break;
        }
    } //  1
    fromx = x;
    fromy = y;
    while (fromx > 0) {
        fromx--;
        if (B[fromx][fromy] == 0) {
            ARROWMOVE(x, y, fromx, fromy, gamer, depth, i);
        } else {
            fromx = x;
            fromy = y;
            break;
        }
    } //   2
    fromx = x;
    fromy = y;
    while (fromx > 0 && fromy < 9) {
        fromx--;
        fromy++;
        if (B[fromx][fromy] == 0) {
            ARROWMOVE(x, y, fromx, fromy, gamer, depth, i);
        } else {
            fromx = x;
            fromy = y;
            break;
        }
    } //   3
    fromx = x;
    fromy = y;
    while (fromy < 9) {
        fromy++;
        if (B[fromx][fromy] == 0) {
            ARROWMOVE(x, y, fromx, fromy, gamer, depth, i);
        } else {
            fromx = x;
            fromy = y;
            break;
        }
    } //   4
    fromx = x;
    fromy = y;
    while (fromx < 9 && fromy < 9) {
        fromx++;
        fromy++;
        if (B[fromx][fromy] == 0) {
            ARROWMOVE(x, y, fromx, fromy, gamer, depth, i);
        } else {
            fromx = x;
            fromy = y;
            break;
        }
    } //   5
    fromx = x;
    fromy = y;
    while (fromx < 9) {
        fromx++;
        if (B[fromx][fromy] == 0) {
            ARROWMOVE(x, y, fromx, fromy, gamer, depth, i);
        } else {
            fromx = x;
            fromy = y;
            break;
        }
    } //   6
    fromx = x;
    fromy = y;
    while (fromx < 9 && fromy > 0) {
        fromx++;
        fromy--;
        if (B[fromx][fromy] == 0) {
            ARROWMOVE(x, y, fromx, fromy, gamer, depth, i);
        } else {
            fromx = x;
            fromy = y;
            break;
        }
    } //   7
    fromx = x;
    fromy = y;
    while (fromy > 0) {
        fromy--;
        if (B[fromx][fromy] == 0) {
            ARROWMOVE(x, y, fromx, fromy, gamer, depth, i);
        } else {
            fromx = x;
            fromy = y;
            break;
        }
    } //  8
}

void chessboard::ARROWMOVE(int FROMX, int FROMY, int fromx, int fromy,
                           int gamer, int depth, int i) {
    int x, y;
    x = fromx;
    y = fromy;
    while (fromx > 0 && fromy > 0) {
        fromx--;
        fromy--;
        if (B[fromx][fromy] == 0) {
            MoveList[depth][COUNT].from.x = FROMX;
            MoveList[depth][COUNT].from.y = FROMY;
            MoveList[depth][COUNT].to.x = x;
            MoveList[depth][COUNT].to.y = y;
            MoveList[depth][COUNT].hinderto.x = fromx;
            MoveList[depth][COUNT].hinderto.y = fromy;
            COUNT++;
        } else {
            fromx = x;
            fromy = y;
            break;
        }
    } //  1
    fromx = x;
    fromy = y;
    while (fromx > 0) {
        fromx--;
        if (B[fromx][fromy] == 0) {
            MoveList[depth][COUNT].from.x = FROMX;
            MoveList[depth][COUNT].from.y = FROMY;
            MoveList[depth][COUNT].to.x = x;
            MoveList[depth][COUNT].to.y = y;
            MoveList[depth][COUNT].hinderto.x = fromx;
            MoveList[depth][COUNT].hinderto.y = fromy;
            COUNT++;
        } else {
            fromx = x;
            fromy = y;
            break;
        }
    } //   2
    fromx = x;
    fromy = y;
    while (fromx > 0 && fromy < 9) {
        fromx--;
        fromy++;
        if (B[fromx][fromy] == 0) {
            MoveList[depth][COUNT].from.x = FROMX;
            MoveList[depth][COUNT].from.y = FROMY;
            MoveList[depth][COUNT].to.x = x;
            MoveList[depth][COUNT].to.y = y;
            MoveList[depth][COUNT].hinderto.x = fromx;
            MoveList[depth][COUNT].hinderto.y = fromy;
            COUNT++;
        } else {
            fromx = x;
            fromy = y;
            break;
        }
    } //   3
    fromx = x;
    fromy = y;
    while (fromy < 9) {
        fromy++;
        if (B[fromx][fromy] == 0) {
            MoveList[depth][COUNT].from.x = FROMX;
            MoveList[depth][COUNT].from.y = FROMY;
            MoveList[depth][COUNT].to.x = x;
            MoveList[depth][COUNT].to.y = y;
            MoveList[depth][COUNT].hinderto.x = fromx;
            MoveList[depth][COUNT].hinderto.y = fromy;
            COUNT++;
        } else {
            fromx = x;
            fromy = y;
            break;
        }
    } //   4
    fromx = x;
    fromy = y;
    while (fromx < 9 && fromy < 9) {
        fromx++;
        fromy++;
        if (B[fromx][fromy] == 0) {
            MoveList[depth][COUNT].from.x = FROMX;
            MoveList[depth][COUNT].from.y = FROMY;
            MoveList[depth][COUNT].to.x = x;
            MoveList[depth][COUNT].to.y = y;
            MoveList[depth][COUNT].hinderto.x = fromx;
            MoveList[depth][COUNT].hinderto.y = fromy;
            COUNT++;
        } else {
            fromx = x;
            fromy = y;
            break;
        }
    } //   5
    fromx = x;
    fromy = y;
    while (fromx < 9) {
        fromx++;
        if (B[fromx][fromy] == 0) {
            MoveList[depth][COUNT].from.x = FROMX;
            MoveList[depth][COUNT].from.y = FROMY;
            MoveList[depth][COUNT].to.x = x;
            MoveList[depth][COUNT].to.y = y;
            MoveList[depth][COUNT].hinderto.x = fromx;
            MoveList[depth][COUNT].hinderto.y = fromy;
            COUNT++;
        } else {
            fromx = x;
            fromy = y;
            break;
        }
    } //   6
    fromx = x;
    fromy = y;
    while (fromx < 9 && fromy > 0) {
        fromx++;
        fromy--;
        if (B[fromx][fromy] == 0) {
            MoveList[depth][COUNT].from.x = FROMX;
            MoveList[depth][COUNT].from.y = FROMY;
            MoveList[depth][COUNT].to.x = x;
            MoveList[depth][COUNT].to.y = y;
            MoveList[depth][COUNT].hinderto.x = fromx;
            MoveList[depth][COUNT].hinderto.y = fromy;
            COUNT++;
        } else {
            fromx = x;
            fromy = y;
            break;
        }
    } //   7
    fromx = x;
    fromy = y;
    while (fromy > 0) {
        fromy--;
        if (B[fromx][fromy] == 0) {
            MoveList[depth][COUNT].from.x = FROMX;
            MoveList[depth][COUNT].from.y = FROMY;
            MoveList[depth][COUNT].to.x = x;
            MoveList[depth][COUNT].to.y = y;
            MoveList[depth][COUNT].hinderto.x = fromx;
            MoveList[depth][COUNT].hinderto.y = fromy;
            COUNT++;
        } else {
            fromx = x;
            fromy = y;
            break;
        }
    } //  8
    // return COUNT;
}
