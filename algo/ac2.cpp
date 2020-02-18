#include<bits/stdc++.h>
using namespace std;

class _AC{
private:
    static const int N = 50010;
    int c[N][2],val[N],fail[N],cnt;
    queue<int>q;
public:
    typedef array<int, 2> PatPair;
    _AC():cnt(0){
        memset(c, 0, sizeof c);
        memset(val, 0, sizeof val);
        memset(fail, 0, sizeof fail);
    }
    void ins(const char* s, int id){
        int len=strlen(s);int now=0;
        for(int i=0;i<len;i++){
            int v= (s[i] == '0' ? 1 : 0);
            if(!c[now][v])c[now][v]=++cnt;
            now=c[now][v];
        }
        val[now] = id;
    }
    void build(){
        for(int i=0;i<2;i++)if(c[0][i])fail[c[0][i]]=0,q.push(c[0][i]);
        while(!q.empty()){
            int u=q.front();q.pop();
            for(int i=0;i<2;i++)
            if(c[u][i])fail[c[u][i]]=c[fail[u]][i],q.push(c[u][i]);
            else c[u][i]=c[fail[u]][i];
            // build a Trie Graph
        }
    }
    vector<PatPair> query(vector<int> s){
        int len=s.size();int now=0;
        vector<PatPair> ans;
        for(int i=0;i<len;i++){
            now = c[now][s[i]]; // jump to the next match node
            for (int t=now; t&&val[t]; t=fail[t]) // follow the "fail" to sum all match pattern
                ans.push_back({i, val[t]});
        }
        return ans;
    }
}AC;
const char* pats[][10]
{
    {0},
    { "0" }, // 1
    {
        "00",
        "0_0"
    }, // 2-3
    { "0__0" }, // 3-2
    { "0___0" }, // 4-1
    { "000" }, // 5
    { "0_00" }, // 6
    {
        "0__00",
        "0_0_0",
    }, // 7
    { "0000" }, // 8
    {
        "000_0",
        "00_00",
    }, // 9
    {   "00000" } // 10
};
// _: 0; 0:1;
int cnt[] = {0, 1, 2, 1, 1, 1, 1, 2, 1, 2, 1};
void reverse(const char* s, char res[])
{
    strcpy(res, s);
    int len = strlen(s);
    for ( int i = 0; i < len / 2; i++)
    {
        swap(res[i], res[len - i - 1]);
    }
}
int main()
{
    char tmp[10];
    for (int i = 1; i <= 10; i++)
    {
        for (int j = 0; j < cnt[i]; j++)
        {
            AC.ins(pats[i][j], i);
            reverse(pats[i][j], tmp);
            AC.ins(tmp, i);
        }
    }
    AC.build();
    vector<int> s = { 
        0,1,0,1,0,1,1,1,0,1,0,0,1 };
      //0,1,2,3,4,5,6,7,8,9,a,b,c
    vector<_AC::PatPair> res = AC.query(s);
    return 0;
}