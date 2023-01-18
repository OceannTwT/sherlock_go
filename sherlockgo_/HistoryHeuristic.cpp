// HistoryHeuristic.cpp: implementation of the CHistoryHeuristic class.

#include "HistoryHeuristic.h"

CHistoryHeuristic::CHistoryHeuristic() {
	// ��ʼ����ά����
    int i, j;
    m_HistoryTable = new int **[100];
    for (i = 0; i < 100; i++) {
        m_HistoryTable[i] = new int *[100];
    }
    for (i = 0; i < 100; i++) {
        for (j = 0; j < 100; j++) {
            m_HistoryTable[i][j] = new int[100];
        }
    }
}

CHistoryHeuristic::~CHistoryHeuristic() {
	//������ά����
    int i, j;
    for (i = 0; i < 100; i++) {
        for (j = 0; j < 100; j++) {
            delete[] m_HistoryTable[i][j];
        }
    }
    for (i = 0; i < 100; i++) {
        delete[] m_HistoryTable[i];
    }
    delete[] m_HistoryTable;
}

void CHistoryHeuristic::ResetHistoryTable() {
    int i, j, k;
    for (i = 0; i < 100; i++) {
        for (j = 0; j < 100; j++) {
            for (k = 0; k < 100; k++) {
                m_HistoryTable[i][j][k] = 168430090; // �൱��������Ŀ����
            }
        }
    }
}

int CHistoryHeuristic::GetHistoryScore(chess *move) {
    int QueenFrom, QueenGo, ArrowTo;

    QueenFrom = move->from.x * 10 + move->from.y;	// ��ʼλ��
    QueenGo = move->to.x * 10 + move->to.y;			// Ŀ��λ��
    ArrowTo = move->hinderto.x * 10 + move->hinderto.y;	// �ż�λ��

    return m_HistoryTable[QueenFrom][QueenGo][ArrowTo]; // ������ʷ�÷�
}

void CHistoryHeuristic::EnterHistoryScore(chess *move, int depth) {
    int QueenFrom, QueenGo, ArrowTo;

    QueenFrom = move->from.x * 10 + move->from.y;
    QueenGo = move->to.x * 10 + move->to.y;
    ArrowTo = move->hinderto.x * 10 + move->hinderto.y;

    m_HistoryTable[QueenFrom][QueenGo][ArrowTo] += 2 << depth; //����Ϊ2��depth�η�
}


void CHistoryHeuristic::Merge(chess *source, chess *target, int l, int m,int r) {
	// �������򣬺ϲ� source[1...m] �� soure[m+1...r] �� target[1...r]
    int i = l;
    int j = m + 1;
    int k = l;

    while ((i <= m) && (j <= r))
        if (source[i].value <= source[j].value)
            target[k++] = source[i++];
        else
            target[k++] = source[j++];

    if (i > m)
        for (int q = j; q <= r; q++)
            target[k++] = source[q];
    else
        for (int q = i; q <= m; q++)
            target[k++] = source[q];
}

void CHistoryHeuristic::Merge_A(chess *source, chess *target, int l, int m,int r) {
	// �ݼ����򣬺ϲ� source[1...m] �� soure[m+1...r] �� target[1...r]
    int i = l;
    int j = m + 1;
    int k = l;

    while ((i <= m) && (j <= r))
        if (source[i].value >= source[j].value)
            target[k++] = source[i++];
        else
            target[k++] = source[j++];

    if (i > m)
        for (int q = j; q <= r; q++)
            target[k++] = source[q];
    else
        for (int q = i; q <= m; q++)
            target[k++] = source[q];
}

void CHistoryHeuristic::MergePass(chess *source, chess *target, const int s,
                                  const int n, const bool direction) {
    // �ϲ���СΪs�����ڶ���������, direction ָ�������ǵݼ����ǵ���
    int i = 0;

    while (i <= n - 2 * s) {
        if (direction)
            Merge(source, target, i, i + s - 1, i + 2 * s - 1);
        else
            Merge_A(source, target, i, i + s - 1, i + 2 * s - 1);
        i = i + 2 * s;
    }

    if (i + s < n) // ʣ���Ԫ�ظ���С��2s
    {
        if (direction)
            Merge(source, target, i, i + s - 1, n - 1);
        else
            Merge_A(source, target, i, i + s - 1, n - 1);
    } else
        for (int j = i; j <= n - 1; j++)
            target[j] = source[j];
}

void CHistoryHeuristic::MergeSort(chess *source, int n, bool direction) {
	// ���ⲿ���õĹ鲢������
    int s = 1;
    while (s < n) {
        MergePass(source, m_TargetBuff, s, n, direction);
        s += s;
        MergePass(m_TargetBuff, source, s, n, direction);
        s += s;
    }
}
