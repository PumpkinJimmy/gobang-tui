#ifndef __MATCH_H
#define __MATCH_H
#include <cstring>
#include <queue>
#include <array>
using namespace std;
class Matcher{
public:
    struct Pat
    {
        int lpos; int rpos; // [lpos, rpos]
        int id;
        int guard;
    };
    Matcher();
    vector<Pat> query(vector<int> s);
private:
    void reverse(const char* s, char res[]);
    void loadPatStr(const char* s, int id);
    void build();
    static const int N = 50010;
    int c[N][3],val[N],fail[N],cnt;
};
#endif