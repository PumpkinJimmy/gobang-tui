#include <cstring>
#include <queue>
#include <array>
#include <iostream>
#include <algorithm>
#define mdid(r,c) (r-c+10)
#define cdid(r,c) (r+c-1)
#define mdpos(r,c) (r+c+2-abs(r-c))/2
#define cdpos(r,c) (r-c+9-abs(r+c-11))/2+1
using namespace std;
const int inf = 0x7ffffff;
const int PatCnt = 13;
const char* pats[PatCnt+1] =
    {
    "",
    "0", // 1
    "00", // 2
    "0_0", // 2
    "0__0", // 3
    "0___0", // 4
    "000", // 5
    "0_00", // 6
    "0__00", // 7
    "0_0_0",// 7
    "000_0",// 8
    "00_00",// 8
    "0000", // 9
    "00000" // 10
};
int info[PatCnt+1][3] = {
    {0},
    {1, 1, 1},
    {2, 2, 2},
    {2, 3, 2},
    {3, 4, 2},
    {4, 5, 2},
    {5, 3, 3},
    {6, 4, 3},
    {7, 5, 3},
    {7, 5, 3},
    {8, 5, 4},
    {8, 5, 4},
    {9, 4, 4},
    {10, 5, 5}
}; // info[x][0]: typeid; [1]: length; [2]: tot
int ptrans[11][3][4] = { {},
    {{2}, {2}}, //1
    {{5, 6}, {6}}, //2
    {{6}, {6}, {6}}, //3
    {{7}, {7}, {7}}, //4
    {{9}, {8}}, //5
    {{8, 9}, {8}}, //6
    {{8}, {8}, {8}}, //7
    {{10}, {10}, {10}}, //8
    {{10}, {10}}, //9
};

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
int eval(const vector<int>& states, Matcher::Pat pat)
{
    switch (pat.id)
    {
    case 10:
        return inf;
    case 9:
        if (pat.guard == 0) return inf / 3;
        else if (pat.guard == 1) return 20;
        else return 0;
    case 8:
        return 20;
    case 7:
        return 10;
    case 6:
        if (pat.guard == 2) return 0;
        else return 10;
    case 5:
        if (pat.guard == 2) return 0;
        else if (pat.guard == 1) return 10;
        else return 15;
    case 4:
        return 3;
    case 3:
        if (pat.guard == 2) return 0;
        else return 3;
    case 2:
        if (pat.guard == 2) return 0;
        else return 4;
    case 1:
        return 0;
    }
}
Matcher::Matcher()
:cnt(0)
{
    memset(c, 0, sizeof c);
    memset(val, 0, sizeof val);
    memset(fail, 0, sizeof fail);
    char tmp[20];
    for (int i = 1; i <= PatCnt; i++)
    {
        for (int j = 0; j < info[i][1]; j++)
        {
            loadPatStr(pats[i], i);
            reverse(pats[i], tmp);
            loadPatStr(tmp, i);
        }
    }
    build();
}
vector<Matcher::Pat> oans, ans;
vector<Matcher::Pat> Matcher::query(const vector<int>& s){
    int len=s.size();int now=0;
    oans.clear(), ans.clear();
    for(int i=0;i<len;i++){
        now = c[now][s[i]]; // jump to the next match node
        if (val[now]){
            int maxv = -1; Pat maxpat;
            for (int t=now; t&&val[t]; t=fail[t]) // follow the "fail" to sum all match pattern
            {
                int id = info[val[t]][0];
                int lpos = i-info[val[t]][1]+1, rpos = i;
                int guard = (s[lpos-1] != 0) + (s[rpos+1] != 0);
                Pat tmp{i - info[val[t]][1] + 1, i, id, guard};
                tmp.v = eval(s, tmp);
                if (tmp.v > maxv) {maxv = tmp.v; maxpat = tmp; }
            }
            oans.push_back(maxpat);
        }
    }
    sort(oans.begin(), oans.end(), greater<Pat>());
    for (int i = 0; i < oans.size(); i++)
    {
        bool ok = true;
        for (int j = i - 1; j >= 0; j--)
        {
            if (oans[j].lpos <= oans[i].lpos && oans[i].rpos <= oans[j].rpos)
            {
                ok = false;
                break;
            }
        }
        if (ok) ans.push_back(oans[i]);
    }
    return ans;
}
void Matcher::reverse(const char* s, char res[])
{
    strcpy(res, s);
    int len = strlen(s);
    for ( int i = 0; i < len / 2; i++)
    {
        swap(res[i], res[len - i - 1]);
    }
}
void Matcher::loadPatStr(const char* s, int id){
    int len=strlen(s);int now=0;
    for(int i=0;i<len;i++){
        int v= (s[i] == '0' ? 1 : 0);
        if(!c[now][v])c[now][v]=++cnt;
        now=c[now][v];
    }
    val[now] = id;
}
void Matcher::build(){
    queue<int>q;
    for(int i=0;i<3;i++)if(c[0][i])fail[c[0][i]]=0,q.push(c[0][i]);
    while(!q.empty()){
        int u=q.front();q.pop();
        for(int i=0;i<3;i++)
        if(c[u][i])fail[c[u][i]]=c[fail[u]][i],q.push(c[u][i]);
        else c[u][i]=c[fail[u]][i];
        // build a Trie Graph
    }
}
int rowcnt = 10, colcnt = 10;
vector<vector<int>> rows[2], cols[2], mdiag[2], cdiag[2];
void setAt(int row, int col, int turn)
{
    int i = row, j = col;
    rows[0][i][j] = turn+1; rows[1][i][j] = (turn^1)+1;
    cols[0][j][i] = turn+1; cols[1][j][i] = (turn^1)+1;
    mdiag[0][mdid(i,j)][mdpos(i,j)] = turn+1; mdiag[1][mdid(i,j)][mdpos(i,j)] = (turn^1)+1; 
    cdiag[0][cdid(i,j)][cdpos(i,j)] = turn+1; cdiag[1][cdid(i,j)][cdpos(i,j)] = (turn^1)+1;
}
void clearAt(int row, int col)
{
    int i = row, j = col;
    rows[0][i][j] = 0; rows[1][i][j] = 0;
    cols[0][j][i] = 0; cols[1][j][i] = 0;
    mdiag[0][mdid(i,j)][mdpos(i,j)] = 0; mdiag[1][mdid(i,j)][mdpos(i,j)] = 0; 
    cdiag[0][cdid(i,j)][cdpos(i,j)] = 0; cdiag[1][cdid(i,j)][cdpos(i,j)] = 0;
}
typedef array<int, 2> Pos;
Matcher matcher;
vector<Matcher::Pat> pat2;
int evalMove(vector<int> states, int origin_bene, int mv)
{
    states[mv] = 1;
    pat2 = matcher.query(states);
    int bene2=0;
    for (auto& pat: pat2) bene2 += pat.v;
    return bene2 - origin_bene;
}
int solve()
{
}
int main()
{
    int maxv = -1, maxi, maxj;
    vector<Matcher::Pat> pats;
    for (int i = 0; i <= 1; i++)
    {
        rows[i].resize(12); for (auto& row: rows[i]) { row.resize(12); row[0] = 2; row[11] = 2; }
        cols[i].resize(12); for (auto& row: cols[i]) { row.resize(12); row[0] = 2; row[11] = 2; }
        mdiag[i].resize(21);
        for (int x = 0; x <= 20; x++) {
            mdiag[i][x].resize(10-abs(10-x)+2);
            mdiag[i][x][0] = 2; mdiag[i][x][10-abs(10-x)+2-1] = 2;
        }
        cdiag[i].resize(21);
        for (int x = 0; x <= 20; x++) {
            cdiag[i][x].resize(10-abs(10-x)+2);
            cdiag[i][x][0] = 2; cdiag[i][x][10-abs(10-x)+2-1] = 2;
        }
        for (auto& x: rows[i][0]) x = 2;
        for (auto& x: rows[i][11]) x = 2;
        for (auto& x: cols[i][0]) x = 2;
        for (auto& x: cols[i][11]) x = 2;

    }
    setAt(5, 5, 0); setAt(4, 5, 1); setAt(4, 4, 1); setAt(4,6,0), setAt(6, 4, 0);
    int ob, bene;
    for (int i = 1; i <= 10; i++)
    {
        for (int j = 1; j <= 10; j++)
        {
            if (rows[0][i][j] != 0) continue;
            bene = 0;

            pats = matcher.query(rows[0][i]);
            ob = 0; for (auto& pat: pats) ob += pat.v;
            bene += evalMove(rows[0][i], ob, j);

            pats = matcher.query(cols[0][j]);
            ob = 0; for (auto& pat: pats) ob += pat.v;
            bene += evalMove(cols[0][j], ob, i);

            pats = matcher.query(mdiag[0][mdid(i,j)]);
            ob = 0; for (auto& pat: pats) ob += pat.v;
            bene += evalMove(mdiag[0][mdid(i,j)], ob, mdpos(i,j));

            pats = matcher.query(cdiag[0][cdid(i,j)]);
            ob = 0; for (auto& pat: pats) ob += pat.v;
            bene += evalMove(cdiag[0][cdid(i,j)], ob, cdpos(i,j));

            if (bene > maxv) maxv = bene, maxi = i, maxj = j;
        }
    }
    cout << maxv << ' ' << maxi << ' ' << maxj << endl;
    // int maxv = 0, maxi, maxj;
    // for (int i = 0; i < 10; i++)
    // {
    //     for (int j = 0; j < 10; j++)
    //     {
    //         if (dfs(i, j, 0, 1, 0) > maxv)
    //         {
    //             maxi = i; maxj = j;
    //         }
    //     }
    // }
    // std::cout << maxv << ' ' << maxi << ' ' << maxj << std::endl;
}