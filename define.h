#ifndef DEFINE_H_INCLUDED
#define DEFINE_H_INCLUDED

#define QueenW 1
#define QueenB -1
#define NOCHESS 0
#define HINDER 2
//#define max_depth 3
//#define TIME 20000

struct position {
    int x;
    int y;
};

struct chess {
    position from, to, hinderto;
    float value;
};

struct wbposition {
    position from; //子位置信息
};

class CDefine {
  public:
    CDefine();
    virtual ~CDefine();
};

#endif // DEFINE_H_INCLUDED
