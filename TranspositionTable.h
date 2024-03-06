// TranspositionTable.h: interface for the CTranspositionTable class.

#ifndef TRANSPOSITIONTABLE_H
#define TRANSPOSITIONTABLE_H

#pragma once
#include "Amazon.h"
#include <ctime>
#define GRID_NUM 10
enum ENTRY_TYPE { exact, lowerbound, upperbound };

typedef struct HASHITEM {
    unsigned long long checksum;
    ENTRY_TYPE entry_type;
    int depth;
    double eval;
} HashItem;

class CTranspositionTable {
  public:
    CTranspositionTable();
    virtual ~CTranspositionTable();
    void CalculateInitHashKey(int CurPosition[GRID_NUM][GRID_NUM]);
    void Hash_MakeMove(chess move, int type);
    void Hash_UnMakeMove(chess move, int type);

    double LookUpHashTable(double alpha, double beta, int depth, int TableNo);
    void EnterHashTable(ENTRY_TYPE entry_type, double eval, int depth,
                        int TableNo);

    unsigned long long m_nHashKey32[3][GRID_NUM][GRID_NUM];
    unsigned long long m_ulHashKey64[3][GRID_NUM][GRID_NUM];
    HashItem *m_pTT[2];
    void InitializeHashKey();
    unsigned long long m_HashKey32;
    unsigned long long m_HashKey64;
};

#endif
