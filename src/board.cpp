#include <ncurses.h>
#include <cstdlib>
#include "board.h"
BoardWin::BoardWin(int rows_, int cols_, int top_, int left_):
top(top_), left(left_), rows(rows_), cols(cols_)
{
    height = rows * 2 + 1;
    width = cols * 3 + 1;
    init_color(COLOR_BLUE, 0, 0, 0);
    init_pair(1, COLOR_CYAN, COLOR_BLUE);
    init_pair(2, COLOR_CYAN, COLOR_WHITE);
    bcolor = 1;
    wcolor = 2;
    win = newwin(height, width, top, left);
    drawBoard();
}
void BoardWin::setAt(int row, int col, Color color)
{
    if (row < 0 || row >= rows || col < 0 || col >= cols)
        throw "Try to render chess at invalid position";
    int grow = row * 2 + 1, gcol = col * 3 + 1;
    int cpair;
    if (color == Color::BLACK)
        cpair = bcolor;
    else if (color == Color::WHITE)
        cpair = wcolor;
    else
        throw "Unknown color of chess";
    
    mvwchgat(win, grow, gcol, 2, A_NORMAL, cpair, NULL);
    wrefresh(win);
}
BoardWin::~BoardWin()
{
    delwin(win);
}
void BoardWin::drawBoard()
{
    // top line
    mvwaddch(win, 0, 0, ACS_ULCORNER);
    for (int i = 1; i <= (width - 1) / 3 - 1; i++)
    {
        waddch(win, ACS_HLINE);
        waddch(win, ACS_HLINE);
        waddch(win, ACS_TTEE);
    }
    waddch(win, ACS_HLINE);
    waddch(win, ACS_HLINE);
    waddch(win, ACS_URCORNER);

    // middle line
    for (int r = 1; r < height - 2; r += 2)
    {
        // block row
        mvwaddch(win, r, 0, ACS_VLINE);
        for (int i = 1; i + 2 <= width - 1; i += 3)
        {
            //mvwchgat(win, r, i, 2, A_REVERSE, 0, NULL);
            mvwaddch(win, r, i+2, ACS_VLINE);
        }
        waddch(win, ACS_VLINE);
        // cross row
        mvwaddch(win, r+1, 0, ACS_LTEE);
        for (int i = 1; i <= (width - 1) / 3 - 1; i++)
        {
            waddch(win, ACS_HLINE);
            waddch(win, ACS_HLINE);
            waddch(win, ACS_PLUS);
        }
        waddch(win, ACS_HLINE);
        waddch(win, ACS_HLINE);
        waddch(win, ACS_RTEE);
    }

    // last block row
    int r = height - 2;
    mvwaddch(win, r, 0, ACS_VLINE);
    for (int i = 1; i + 2 <= width - 1; i += 3)
    {
        //mvwchgat(win, r, i, 2, A_REVERSE, 0, NULL);
        mvwaddch(win, r, i+2, ACS_VLINE);
    }
    waddch(win, ACS_VLINE);
    // bottom line
    mvwaddch(win, height-1, 0, ACS_LLCORNER);
    for (int i = 1; i < width - 1; i += 3)
    {
        waddch(win, ACS_HLINE);
        waddch(win, ACS_HLINE);
        waddch(win, ACS_BTEE);
    }
    waddch(win, ACS_HLINE);
    waddch(win, ACS_HLINE);
    waddch(win, ACS_LRCORNER);
    wrefresh(win);
}

WINDOW* createBoard(int height, int width, int top, int left)
{
    width = width * 3 + 1;
    height = height * 2 + 1;
    WINDOW* win = newwin(height, width, top, left);
    mvwaddch(win, 0, 0, ACS_ULCORNER);
    for (int i = 1; i < width - 1; i += 3)
    {
        waddch(win, ACS_HLINE);
        waddch(win, ACS_HLINE);
        waddch(win, ACS_TTEE);
    }
    waddch(win, ACS_URCORNER);
    //mvwchgat(win, 1, 1, 2, A_REVERSE, 0, NULL);
    for (int r = 1; r < height - 1; r += 2)
    {
        mvwaddch(win, r, 0, ACS_VLINE);
        for (int i = 1; i < width - 1; i += 3)
        {
            //mvwchgat(win, r, i, 2, A_REVERSE, 0, NULL);
            mvwaddch(win, r, i+2, ACS_VLINE);
        }
        mvwaddch(win, r+1, 0, ACS_LTEE);
        for (int i = 2; i < width - 1; i += 3)
        {
            waddch(win, ACS_HLINE);
            waddch(win, ACS_HLINE);
            waddch(win, ACS_PLUS);
        }
        mvwaddch(win, r, width - 1, ACS_VLINE);
        mvwaddch(win, r+1, width - 1, ACS_RTEE);
    }
    mvwaddch(win, height-1, 0, ACS_LLCORNER);
    for (int i = 1; i < width - 1; i += 3)
    {
        waddch(win, ACS_HLINE);
        waddch(win, ACS_HLINE);
        waddch(win, ACS_BTEE);
    }
    waddch(win, ACS_LRCORNER);
    wrefresh(win);
    return win;
}