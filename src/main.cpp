#include <ncurses.h>
#include "board.h"
using namespace std;

int main()
{
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    curs_set(0);
    start_color();
    init_color(COLOR_BLACK, 100, 100, 700);
    init_color(COLOR_WHITE, 1000, 1000, 1000);

    attron(A_BOLD);
    mvprintw(0, (COLS - 11)/2, "Gobang Game");
    attroff(A_BOLD);
    refresh();
    BoardWin board(10, 10, (LINES - 21)/2, (COLS - 31)/2);
    board.setAt(0, 0, BoardWin::BLACK);
    board.setAt(1, 0, BoardWin::WHITE);
    refresh();
    getch();

    endwin();
    return 0;
}