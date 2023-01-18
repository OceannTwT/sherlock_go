// HistoryHeuristic.h: interface for the CHistoryHeuristic class.

#ifndef HISTORYHEURISTIC_H
#define HISTORYHEURISTIC_H

#pragma once
#include "Amazon.h"
class CHistoryHeuristic {
  public:
    CHistoryHeuristic();
    virtual ~CHistoryHeuristic();

	// �����ʷ��¼��
    void ResetHistoryTable();

	// ȡĳһ�߷�����ʷ�÷�
    int GetHistoryScore(chess *move);

	// ��ĳһ����߷�������ʷ��¼��
    void EnterHistoryScore(chess *move, int depth);

	// �Ե�ǰ�߷����н��й鲢����
    void MergeSort(chess *source, int n, bool direction);

  protected:
	// �ϲ�����õ��������ݶΣ�MergeSort ����
    void Merge(chess *source, chess *target, int l, int m, int r);

	// ��С��������MergeSort ����
    void MergePass(chess *source, chess *target, const int s, const int n,const bool direction);

	// �Ӵ�С����MergeSort ����
    void Merge_A(chess *source, chess *target, int l, int m, int r);

    chess m_TargetBuff[3000]; // �����õĻ�������
    int ***m_HistoryTable;    // ��ʷ�÷ֱ�
};

#endif
