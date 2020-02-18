#include <cstring>
#include <queue>
#include <array>
#include <match.h>
using namespace std;
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
            loadPatStr(pats[i], info[i][0]);
            reverse(pats[i], tmp);
            loadPatStr(tmp, info[i][0]);
        }
    }
    build();
}
vector<Matcher::Pat> Matcher::query(vector<int> s){
    int len=s.size();int now=0;
    vector<Pat> ans;
    for(int i=0;i<len;i++){
        now = c[now][s[i]]; // jump to the next match node
        for (int t=now; t&&val[t]; t=fail[t]) // follow the "fail" to sum all match pattern
        {
            int id = val[t];
            int lpos = i-info[id][1]+1, rpos = i;
            int guard = (s[lpos] != 0) + (s[rpos] != 0);
            ans.push_back({i - info[id][1] + 1, i, id});
        }
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