#include <iostream>
#include <vector>
#include <array>
using namespace std;
class Board
{
public:
    typedef array<int, 2> Pos;
    enum State
    {
        E = 0,
        B = 1,
        W = 2,
        X = 3
    };
    Board(int _rows, int _cols) : rows(_rows), cols(_cols)
    {
        states.resize(rows + 2);
        for (auto &vec : states)
        {
            vec.resize(cols + 2);
        }
        for (auto &state : states[0])
        {
            state = W;
        }
        for (auto &state : states[rows + 1])
        {
            state = W;
        }
        for (int i = 1; i <= rows; i++)
        {
            states[i][0] = W;
            states[i][cols + 1] = W;
        }
    }
    void setAt(Pos pos, State s)
    {
        states[pos[0]][pos[1]] = s;
    }
    bool judge(int tot,
               State sl, State sr, Pos pl, Pos pr,
               vector<Pos> &res_ps)
    {
        if (tot == 4)
        {
            if (sl == E && sr == E)
                return false;
            else if (sl == W && sr == W)
                return true;
            else if (sl == W && sr == E)
                res_ps.push_back(pr);
            else
                res_ps.push_back(pl);
            return true;
        }
        else if (tot == 3)
        {
            if (sl == E && sr == E)
                res_ps.push_back(pl);
                res_ps.push_back(pr);
            return true;
        }
        return true;
    }
    bool judge(int tot, Pos emp_pos,
               State sl, State sr, Pos pl, Pos pr,
               vector<Pos> &res_ps)
    {
        if (tot == 4)
        {
            res_ps.push_back(emp_pos);
            return true;
        }
        else if (tot == 3)
        {
            if (sl == E && sr == E)
                res_ps.push_back(pl);
            res_ps.push_back(pr);
            res_ps.push_back(emp_pos);
            return true;
        }
    }

    bool solveRow(Pos pos, vector<Pos> &res_ps)
    {
        // init
        res_ps.clear();
        vector<Pos> tmp1, tmp2;
        int row = pos[0], col = pos[1];
        int l = col - 1, r = col + 1;
        int emp = 0; bool ok = true;

        // expand range
        while (states[row][l] == B)
            l--;
        while (states[row][r] == B)
            r++;
        
        // expand again
        if (r - l - 1 <= 3)
        {
            if (states[row][r] == E)
            {
                int rr = r + 1;
                while (states[row][rr] == B)
                    rr++;
                if (rr > r + 1) // inner empty pattern
                {
                    emp |= 1;
                    ok = ok && judge(rr - l - 2, {row, r},
                          states[row][l], states[row][rr], {row, l}, {row, rr}, tmp1);
                }
            }
            if (states[row][l] == E)
            {
                int ll = l - 1;
                while (states[row][ll] == B)
                    ll--;
                if (ll < l - 1) // inner empty pattern
                {
                    emp |= 2;
                    ok = ok && judge(r - ll - 2, {row, l},
                      states[row][ll], states[row][r], {row, ll}, {row, r}, tmp2);
                }
            }
        }
        if (emp == 0) // no expand
        {
            ok = ok && judge(r - l - 1, states[row][l], states[row][r], {row, l}, {row, r}, res_ps);
            return ok;
        }
        else if (emp == 1) // set 1
        {
            res_ps = tmp1;
            return ok;
        }
        else if (emp == 2) // set 2
        {
            res_ps = tmp2;
            return ok;
        }
        else // set1 union set2
        {
            for (int i = 0; i < tmp1.size(); i++)
            {
                for (int j = 0; j < tmp2.size(); j++)
                {
                    if (tmp1[i][1] == tmp2[j][1])
                        res_ps.push_back(tmp1[i]);
                }
            }
            return ok && bool(res_ps.size());
        }
    }

private:
    int rows, cols;
    vector<vector<State>> states;
};

int main()
{
    Board board(10, 10);
    // board.setAt({1, 1}, Board::W);
    board.setAt({1, 2}, Board::B);
    board.setAt({1, 3}, Board::B);
    board.setAt({1, 4}, Board::B);
    board.setAt({1, 6}, Board::B);
    vector<Board::Pos> res_ps;
    cout << board.solveRow({1, 3}, res_ps) << endl;
    for (auto pos : res_ps)
    {
        cout << pos[0] << ' ' << pos[1] << endl;
    }
    return 0;
}