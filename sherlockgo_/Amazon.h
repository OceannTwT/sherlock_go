#ifndef AMAZON_H_INCLUDED
#define AMAZON_H_INCLUDED
#include<iostream>
#include<fstream>
#include<vector>
#include<string>
#include<windows.h>
#include<time.h>
#include<graphics.h>
#include<cmath>
#include"define.h"
#include "TranspositionTable.h"
#include"HistoryHeuristic.h"
using namespace std;
extern int mode_choose;
extern int behind;
class chessboard{
public:
	int steps;
    chessboard();
	//???????
	void StartScreen();
	void InitialQipan();
	void StartGame();

    bool ischesslegal(int x,int y,int z, int k,int gamer);//?¡ì????????????
    bool ishinderlegal(int x,int y);//???
    bool iswin();//?¡ì?????
    int where(int x,int y,int z, int k);//?¡ì?????????
    bool leftup(int x,int y,int z, int k);//????
    bool up(int x,int y,int z, int k);//??
    bool rightup(int x,int y,int z, int k);//????
    bool right(int x,int y,int z, int k);//??
    bool rightdown(int x,int y,int z, int k);//????
    bool down(int x,int y,int z, int k);//??
    bool leftdown(int x,int y,int z, int k);//????
    bool left(int x,int y,int z, int k);//??
    int  creatmove(int gamer,int depth);//???????
    void hindermove(chess m,int gamer,int depth);//??????????
    void domove(chess m);//????????
    void unmove(chess m);//???????
    void DOMOVE(chess m);//???????
    void UNMOVE(chess m);//???????
    void output();//???????
    float alpha_beta(float alpha,float beta,int depth,int gamer);
    float psv_white(int depth,float alpha,float beta,int gamer);
    //float Value(vector<chess> &road,int gamer);
    void CvsP();//??????
    void PvsC();//??????
    void output1();
    void OUTPUT();

	double ww;
	int board[144];
	int queen[8];
	int b_analyze[2][144], dist_analyze[2][144];
	void Weight(double ww, double &w1, double &w2, double &w3, double &w4);
	void init_dist1(int p);//???b_analyze[2][144]
	void init_dist2(int p);//???dist_analyze[2][144]
	double Evaluate();//????????

	int b1[144];
	int b2[144];

private:
    int B[10][10];
    chess move;
    wbposition WBposition[2][4];
    int DEPTH;
	int TIME;
    double TimeCount;
    int gamer;
    int count;
    chess MoveList[8][3000];
    int COUNT;
    int m_progress;
    int max_depth;
    double timecount;
    double alltime;
	

public:
    int CREATEMOVE(int depth,int gamer);
    void CHESSMOVE(int fromx,int fromy,int gamer,int depth,int i);
    void ARROWMOVE(int FROMX,int FROMY,int fromx,int fromy,int gamer,int depth,int i);
};


#endif // AMAZON_H_INCLUDED
