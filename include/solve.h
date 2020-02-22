#include "match.h"
#include "algorithm"
#include <iostream>
using namespace std;
const int inf = 0x7ffffff;

static const char* pats[13+1] =
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
static int ptrans[11][3][4] = { {},
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
int val[11] = {0, 1, 2, 3, 4, 5, 6, 7, 8, inf / 3, inf};
class Solver
{
public:
    Solver(){};
    int solve(const vector<int>& states, const vector<int>& estates, int& benefit)
    {
        // auto pats = matcher.query(states);
        // auto epats = matcher.query(estates);
        // sort(pats.begin(), pats.end(), [&](const Matcher::Pat& a, const Matcher::Pat& b)->bool{
        //     return val[a.id] > val[b.id];
        // });
        // sort(epats.begin(), epats.end(), [&](const Matcher::Pat& a, const Matcher::Pat& b)->bool{
        //     return val[a.id] > val[b.id];
        // });
        // benefit = 0;
        // for (auto& pat: pats)
        // {
        //     switch(pat.id)
        //     {
        //     case 9:
        //         if (pat.guard == 2) continue;
        //         else{
        //             benefit = inf;
        //             return states[pat.lpos] == 2 ? pat.rpos : pat.lpos;
        //         }
        //     case 8:
        //         benefit = inf;
        //         for (int i = pat.lpos; i <= pat.rpos; i++)
        //         {
        //             if (states[i] == 0)
        //                 return i;
        //         }
        //     case 7:
        //         benefit = 0; int maxbi;
        //         for (int i = pat.lpos; i <= pat.rpos; i++)
        //         {
        //             if (states[i] == 0)
        //             {
        //                 vector<int> tmp1 = states, tmp2 = estates;
        //                 int tmp3 = 0;
        //                 tmp1[i] = 1; tmp2[i] = 2;
        //                 int res = solve(tmp2, tmp1, tmp3);
        //                 if (tmp3 > benefit)
        //                 {
        //                     benefit = tmp3;
        //                     maxbi = i;
        //                 }
        //             }
        //         }
        //         return maxbi;
        //     case 6:
        //         if (pat.guard == 2) continue;
        //         else if (pat.guard == 1)
        //         {
        //             benefit = 0; int maxbi; int emp = 0;
        //             for (int i = pat.lpos; i <= pat.rpos; i++)
        //             {
        //                 if (states[i] == 0)
        //                 {
        //                     emp = i;
        //                     break;
        //                 }
        //             }
        //             vector<int> tmp1 = states, tmp2 = estates;
        //             int tmp3 = 0;
        //             tmp1[emp] = 1; tmp2[emp] = 2;
        //             int res = solve(tmp2, tmp1, tmp3);
        //             if (tmp3 > benefit)
        //             {
        //                 benefit = tmp3;
        //                 maxbi = i;
        //             }
        //         }
        //         if (pat.guard == 0)
        //         {
        //             benefit = inf / 2;
        //             for (int i = pat.lpos; i<= pat.rpos; i++)
        //                 if (states[i] == 0){
        //                     vector<int> tmp1 = states, tmp2 = estates;
        //                     int tmp3 = 0;
        //                     tmp1[i] = 1; tmp2[i] = 2;
        //                     int res = solve(tmp2, tmp1, tmp3);
        //                     if (tmp3 > benefit)
        //                     {
        //                         benefit = tmp3;
        //                         maxbi = i;
        //                     }
        //                 }

        //         }
        //     }
        // }
        return 0;
    }
private:
    Matcher matcher;
};