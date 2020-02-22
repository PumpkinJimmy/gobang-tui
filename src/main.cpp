#include <ncurses.h>
#include <vector>
#include "board.h"
#include "status.h"
#include "match.h"
#include "game.h"
#include <iostream>
using namespace std;

int main()
{
//     vector<int> vec = {2, 0, 0, 0, 0, 0, 2, 1, 0, 0, 0, 0, 0, 0, 0, 2};
//     Matcher matcher; vector<Matcher::Pat> res = matcher.query(vec);
//     int bene = 0; for (auto& pat: res) bene += pat.v;
//     // for (auto pat: res)
//     // {
//     //     cout << pat.lpos << ' ' << pat.rpos << ' ' << pat.id << ' ' << pat.guard << endl;
//     // }
//     for (int i = 1; i <= vec.size(); i++)
//     {
//         if (vec[i] == 0) std::cout << i << ' ' << evalMove(vec, bene, i) << std::endl;
//     }

    attron(A_BOLD);
    mvprintw(0, (COLS - 11) / 2, "Gobang Game");
    attroff(A_BOLD);
    
    refresh();
    Board board(10, 10);
    BoardWin bwin(10, 10, (LINES-21)/2, (COLS-31)/2);
    BoardRenderer renderer(&bwin, &board);
    Controller controller(&board, &renderer);
    JudgeStrategy judge(board);
    GobangGame* app = GobangGame::getInstance();
    app->setRenderer(&renderer);
    app->setBoard(&board);
    app->setController(&controller);
    app->setJudge(&judge);
    app->run();
    
    getch();

    endwin();
    return 0;
}