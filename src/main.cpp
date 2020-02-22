#include <ncurses.h>
#include <vector>
#include "board.h"
#include "status.h"
#include "match.h"
#include <iostream>
using namespace std;

int main()
{
    vector<int> vec = {2, 0, 0, 0, 0, 0, 2, 1, 0, 0, 0, 0, 0, 0, 0, 2};
    Matcher matcher; vector<Matcher::Pat> res = matcher.query(vec);
    int bene = 0; for (auto& pat: res) bene += pat.v;
    // for (auto pat: res)
    // {
    //     cout << pat.lpos << ' ' << pat.rpos << ' ' << pat.id << ' ' << pat.guard << endl;
    // }
    for (int i = 1; i <= vec.size(); i++)
    {
        if (vec[i] == 0) std::cout << i << ' ' << evalMove(vec, bene, i) << std::endl;
    }
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    curs_set(0);
    start_color();
    init_color(COLOR_BLUE, 100, 100, 700);
    init_color(COLOR_BLACK, 0, 0, 0);
    assume_default_colors(COLOR_WHITE, COLOR_BLUE); // background color

    attron(A_BOLD);
    mvprintw(0, (COLS - 11) / 2, "Gobang Game");
    attroff(A_BOLD);
    refresh();
    BoardWin bwin(10, 10, (LINES - 21) / 2, (COLS - 31) / 2);
    Board board(10, 10, &bwin);
    chtype ch;
    int currow = 0, curcol = 0;
    bwin.setAt(currow, curcol, COLOR_YELLOW);
    refresh();
    while (1)
    {
        ch = getch();
        if (!board.handle(ch))
        {
        }
        if (board.isWin()) { break; }
        board.render();
    }
    getch();

    endwin();
    return 0;
}