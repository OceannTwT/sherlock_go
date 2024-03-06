// TranspositionTable.cpp: implementation of the CTranspositionTable class.
//
//////////////////////////////////////////////////////////////////////

#include "TranspositionTable.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

long long rand64() //生成64位随机数的函数
{
    return rand() ^ ((long long)rand() << 15) ^ ((long long)rand() << 30) ^
           ((long long)rand() << 45) ^ ((long long)rand() << 60);
}
long long rand32() //生成32位随机数的函数
{
    return rand() ^ ((long long)rand() << 15) ^ ((long long)rand() << 30);
    ;
}

CTranspositionTable::CTranspositionTable() { InitializeHashKey(); }

CTranspositionTable::~CTranspositionTable() {
    delete m_pTT[0];
    delete m_pTT[1];
}

void CTranspositionTable::InitializeHashKey() {
    int i, j, k;
    srand((unsigned)time(NULL));
    for (k = 0; k < 3; k++)
        for (i = 0; i < GRID_NUM; i++)
            for (j = 0; j < GRID_NUM; j++) {
                m_nHashKey32[k][i][j] = rand32();
                m_ulHashKey64[k][i][j] = rand64();
            }
    m_pTT[0] = new HashItem[1024 * 10240];
    m_pTT[1] = new HashItem[1024 * 10240];
}

//置换表哈希计算
void CTranspositionTable::CalculateInitHashKey(
    int CurPosition[GRID_NUM][GRID_NUM]) {
    int i, j, nStoneType;
    m_HashKey32 = 0;
    m_HashKey64 = 0;
    for (i = 0; i < GRID_NUM; i++)
        for (j = 0; j < GRID_NUM; j++) {
            nStoneType = CurPosition[i][j];

            if (nStoneType != NOCHESS) {
                if (nStoneType == 1) {
                    nStoneType = 0;
                }
                if (nStoneType == -1) {
                    nStoneType = 1;
                }
                m_HashKey32 = m_HashKey32 ^ m_nHashKey32[nStoneType][i][j];
                m_HashKey64 = m_HashKey64 ^ m_ulHashKey64[nStoneType][i][j];
            }
        }
}

void CTranspositionTable::Hash_MakeMove(chess move, int type) {
    if (type == 1) {
        type = 0;
    } else {
        type = 1;
    }
    m_HashKey32 = m_HashKey32 ^ m_nHashKey32[type][move.from.x][move.from.y];
    m_HashKey64 = m_HashKey64 ^ m_ulHashKey64[type][move.from.x][move.from.y];
    m_HashKey32 = m_HashKey32 ^ m_nHashKey32[type][move.to.x][move.to.y];
    m_HashKey64 = m_HashKey64 ^ m_ulHashKey64[type][move.to.x][move.to.y];
    m_HashKey32 =
        m_HashKey32 ^ m_nHashKey32[HINDER][move.hinderto.x][move.hinderto.y];
    m_HashKey64 =
        m_HashKey64 ^ m_ulHashKey64[HINDER][move.hinderto.y][move.hinderto.x];
}

void CTranspositionTable::Hash_UnMakeMove(chess move, int type) {
    if (type == 1) {
        type = 0;
    } else {
        type = 1;
    }
    m_HashKey32 =
        m_HashKey32 ^ m_nHashKey32[HINDER][move.hinderto.x][move.hinderto.y];
    m_HashKey64 =
        m_HashKey64 ^ m_ulHashKey64[HINDER][move.hinderto.y][move.hinderto.x];
    m_HashKey32 = m_HashKey32 ^ m_nHashKey32[type][move.to.x][move.to.y];
    m_HashKey64 = m_HashKey64 ^ m_ulHashKey64[type][move.to.x][move.to.y];
    m_HashKey32 = m_HashKey32 ^ m_nHashKey32[type][move.from.x][move.from.y];
    m_HashKey64 = m_HashKey64 ^ m_ulHashKey64[type][move.from.x][move.from.y];
}

double CTranspositionTable::LookUpHashTable(double alpha, double beta,
                                            int depth, int TableNo) {
    int x;
    HashItem *pht;
    x = m_HashKey32 % (1024 * 10240);
    pht = &m_pTT[TableNo][x];
    if (pht->depth == depth && pht->checksum == m_HashKey64) {
        switch (pht->entry_type) {
        case exact:
            return pht->eval;
        case lowerbound:
            if (pht->eval >= beta)
                return (pht->eval);
            else
                break;
        case upperbound:
            if (pht->eval <= alpha)
                return (pht->eval);
            else
                break;
        }
    }
    return 66666.0;
}
void CTranspositionTable::EnterHashTable(ENTRY_TYPE entry_type, double eval,
                                         int depth, int TableNo) {
    int x;
    HashItem *pht;
    x = m_HashKey32 % (1024 * 10240);
    pht = &m_pTT[TableNo][x];
    pht->checksum = m_HashKey64;
    pht->entry_type = entry_type;
    pht->eval = eval;
    pht->depth = depth;
}
