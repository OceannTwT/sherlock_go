// HistoryHeuristic.h: interface for the CHistoryHeuristic class.

#ifndef HISTORYHEURISTIC_H
#define HISTORYHEURISTIC_H

#pragma once
#include "Amazon.h"
class CHistoryHeuristic {
  public:
    CHistoryHeuristic();
    virtual ~CHistoryHeuristic();

	// 清空历史记录表
    void ResetHistoryTable();

	// 取某一走法的历史得分
    int GetHistoryScore(chess *move);

	// 将某一最佳走法汇入历史记录表
    void EnterHistoryScore(chess *move, int depth);

	// 对当前走法序列进行归并排序
    void MergeSort(chess *source, int n, bool direction);

  protected:
	// 合并排序好的相邻数据段，MergeSort 调用
    void Merge(chess *source, chess *target, int l, int m, int r);

	// 从小到大排序，MergeSort 调用
    void MergePass(chess *source, chess *target, const int s, const int n,const bool direction);

	// 从大到小排序，MergeSort 调用
    void Merge_A(chess *source, chess *target, int l, int m, int r);

    chess m_TargetBuff[3000]; // 排序用的缓冲序列
    int ***m_HistoryTable;    // 历史得分表
};

#endif
