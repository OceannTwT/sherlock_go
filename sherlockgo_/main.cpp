#define _CRT_SECURE_NO_WARNINGS
#include "Amazon.h"
#include <fstream>
#include <graphics.h> // ͼ�ο⣬���Դ�����װeasyX����
#include <iostream>
#include <stdlib.h>
#include <string>
using namespace std;
int mode_choose = 0;
int behind = 0;

int main() {
	chessboard A;

	initgraph(700, 550, SHOWCONSOLE); // ��ʼ��ͼ�λ���
	// HWND initgraph(int width, int height, int flag = NULL);

	A.StartScreen();  // ������Ļ
	A.InitialQipan(); // �������̣����ӣ����ӣ����ӣ�

	MOUSEMSG m; // �����Ϣ
	cout << "1.(first)" << endl; // ����
	cout << "2.(behind)" << endl; // ����

	// ���ѡ�񣬷�Χ�ж�
	while (mode_choose == 0) {
		m = GetMouseMsg(); // ��ȡһ�������Ϣ�����û�У��͵ȴ�
		if (m.uMsg == WM_LBUTTONDOWN) { // �������
			if (m.x >= 550 && m.x <= 665 && m.y >= 100 && m.y <= 167) {
				mode_choose = 1;
			}
			if (m.x >= 550 && m.x <= 665 && m.y >= 200 && m.y <= 267) {
				mode_choose = 2;
			}
		}
	}

	freopen("process.txt", "a", stdout); // �ض�������������

    if (mode_choose == 1) {
        cout << "�ҷ����֣�ִ��" << endl;
        A.CvsP();
    }
    if (mode_choose == 2) {
        cout << "�ҷ����֣�ִ��" << endl;
        behind = 1;
        A.PvsC();
    }

    // system("pause");
    return 0;
}
