#define _CRT_SECURE_NO_WARNINGS
#include "Amazon.h"
#include <fstream>
#include <graphics.h> // 图形库，不自带，安装easyX附加
#include <iostream>
#include <stdlib.h>
#include <string>
using namespace std;
int mode_choose = 0;
int behind = 0;

int main() {
	chessboard A;

	initgraph(700, 550, SHOWCONSOLE); // 初始化图形环境
	// HWND initgraph(int width, int height, int flag = NULL);

	A.StartScreen();  // 绘制屏幕
	A.InitialQipan(); // 绘制棋盘（黑子，白子，格子）

	MOUSEMSG m; // 鼠标消息
	cout << "1.(first)" << endl; // 先手
	cout << "2.(behind)" << endl; // 后手

	// 点击选择，范围判断
	while (mode_choose == 0) {
		m = GetMouseMsg(); // 获取一个鼠标消息。如果没有，就等待
		if (m.uMsg == WM_LBUTTONDOWN) { // 左键按下
			if (m.x >= 550 && m.x <= 665 && m.y >= 100 && m.y <= 167) {
				mode_choose = 1;
			}
			if (m.x >= 550 && m.x <= 665 && m.y >= 200 && m.y <= 267) {
				mode_choose = 2;
			}
		}
	}

	freopen("process.txt", "a", stdout); // 重定向下棋过程输出

    if (mode_choose == 1) {
        cout << "我方先手，执白" << endl;
        A.CvsP();
    }
    if (mode_choose == 2) {
        cout << "我方后手，执黑" << endl;
        behind = 1;
        A.PvsC();
    }

    // system("pause");
    return 0;
}
