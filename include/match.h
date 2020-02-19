#ifndef __MATCH_H
#define __MATCH_H
#include <cstring>
#include <queue>
#include <array>
#include "board.h"
using namespace std;
class Matcher{
public:
    struct Pat
    {
        int lpos; int rpos; // [lpos, rpos]
        int id;
        int guard;
        int v;
        bool operator>(const Pat& other) const { return v > other.v; }
    };
    Matcher();
    vector<Pat> query(const vector<int>& s);
private:
    void reverse(const char* s, char res[]);
    void loadPatStr(const char* s, int id);
    void build();
    static const int N = 50010;
    int c[N][3],val[N],fail[N],cnt;
};
int evalMove(vector<int> states, int, int mv);
#endif