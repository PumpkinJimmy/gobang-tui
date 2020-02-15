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
        switch (ch)
        {
        case KEY_LEFT:
            if (curcol == 0)
                break;
            bwin.setAt(currow, curcol, COLOR_INVIS);
            bwin.setAt(currow, --curcol, COLOR_YELLOW);
            break;
        case KEY_RIGHT:
            if (curcol == 9)
                break;
            bwin.setAt(currow, curcol, COLOR_INVIS);
            bwin.setAt(currow, ++curcol, COLOR_YELLOW);
            break;
        case KEY_UP:
            if (currow == 0)
                break;
            bwin.setAt(currow, curcol, COLOR_INVIS);
            bwin.setAt(--currow, curcol, COLOR_YELLOW);
            break;
        case KEY_DOWN:
            if (currow == 9)
                break;
            bwin.setAt(currow, curcol, COLOR_INVIS);
            bwin.setAt(++currow, curcol, COLOR_YELLOW);
            break;
        case '\n': // Enter
            mvprintw(LINES - 1, 0, "Choose (%d, %d)", currow, curcol);
            refresh();
            break;
        default:
            break;
        }
    }
    getch();

    endwin();
    return 0;
}