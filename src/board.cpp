#include <ncurses.h>
#include <cstdlib>
#include "board.h"
#include "status.h"
BoardWin::BoardWin(int rows_, int cols_, int top_, int left_) : top(top_), left(left_), rows(rows_), cols(cols_)
{
    height = rows * 2 + 1;
    width = cols * 3 + 1;
    bgid = 0;
    for (int i = 0; i < 8; i++)
    {
        init_pair(i + 1, COLOR_BLUE, i);
        cpairs[i] = i + 1;
    }
    win = newwin(height, width, top, left);
    drawBoard();
}
void BoardWin::setAt(int row, int col, int color)
{
    if (row < 0 || row >= rows || col < 0 || col >= cols)
        throw "Try to render chess at invalid position";
    int grow = row * 2 + 1, gcol = col * 3 + 1;
    if (color == -1) // clear
        mvwchgat(win, grow, gcol, 2, A_NORMAL, bgid, NULL);
    else
        mvwchgat(win, grow, gcol, 2, A_NORMAL, cpairs[color], NULL);
    wrefresh(win);
}
BoardWin::~BoardWin()
{
    delwin(win);
}
void BoardWin::drawBoard()
{
    attron(COLOR_PAIR(bgid));
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
            mvwaddch(win, r, i + 2, ACS_VLINE);
        }
        waddch(win, ACS_VLINE);
        // cross row
        mvwaddch(win, r + 1, 0, ACS_LTEE);
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
        mvwaddch(win, r, i + 2, ACS_VLINE);
    }
    waddch(win, ACS_VLINE);
    // bottom line
    mvwaddch(win, height - 1, 0, ACS_LLCORNER);
    for (int i = 1; i < width - 1; i += 3)
    {
        waddch(win, ACS_HLINE);
        waddch(win, ACS_HLINE);
        waddch(win, ACS_BTEE);
    }
    waddch(win, ACS_HLINE);
    waddch(win, ACS_HLINE);
    waddch(win, ACS_LRCORNER);
    attroff(COLOR_PAIR(bgid));
    wrefresh(win);
}
Board::Board(int rows_, int cols_, BoardWin *win)
:rows(rows_), cols(cols_), bwin(win), 
currow(0), curcol(0), curturn(B),
last_row(-1), last_col(-1), is_win(false)
{
    states.resize(rows);
    for (auto &vec : states)
        vec.resize(cols);
}
const Board::Mat &Board::getMat() const
{
    return states;
}
bool Board::setAt(int row, int col, State turn)
{
    if (row < 0 || row > rows - 1 ||
        col < 0 || col > cols - 1)
        throw "Row Set Overflow";
    else if (states[row][col] != E)
        return false;
    else
    {
        states[row][col] = int(turn);
        bwin->setAt(row, col, cid[int(turn)]);
        return true;
    }
}
bool Board::handle(chtype key)
{
    int c;
    switch (key)
    {
    case 0x1b: //ESC to exit
        exit(0);
    case KEY_LEFT:
        if (curcol == 0)
            return true;
        bwin->setAt(currow, curcol, cid[states[currow][curcol]]);
        curcol--;
        c = states[currow][curcol] == E ? SE : ISE;
        bwin->setAt(currow, curcol, cid[c]);
        return true;
    case KEY_RIGHT:
        if (curcol == cols - 1)
            return true;
        bwin->setAt(currow, curcol, cid[states[currow][curcol]]);
        curcol++;
        c = states[currow][curcol] == E ? SE : ISE;
        bwin->setAt(currow, curcol, cid[c]);
        return true;
    case KEY_UP:
        if (currow == 0)
            return true;
        bwin->setAt(currow, curcol, cid[states[currow][curcol]]);
        currow--;
        c = states[currow][curcol] == E ? SE : ISE;
        bwin->setAt(currow, curcol, cid[c]);
        return true;
    case KEY_DOWN:
        if (currow == rows-1)
            return true;
        bwin->setAt(currow, curcol, cid[states[currow][curcol]]);
        currow++;
        c = states[currow][curcol] == E ? SE : ISE;
        bwin->setAt(currow, curcol, cid[c]);
        return true;
    case '\n': // 'ENTER to input'
        if (states[currow][curcol] == E)
        {
            setAt(currow, curcol, curturn);
            curturn = (curturn == B ? W : B);
            last_row = currow;
            last_col = curcol;
            if (judgeCurrent())
            {
                char str[100];
                sprintf(str, "%s WIN THE GAME", (curturn == W ? "BLACK" : "WHITE"));
                StatusBar::getInstance()->setText(str);
                is_win = true;
            }
        }
        return true;
    default:
        return false;
    }
}
void Board::render()
{
    char str[100];
    sprintf(str, "(%d, %d) %s", currow, curcol, 
    curturn == B ? "BLACK" : "WHITE"
    );
    StatusBar::getInstance()->setText(str);
}
int Board::getRows() const { return rows; }
int Board::getCols() const { return cols; }
Board::State Board::getTurn() const { return curturn; };
bool Board::judgeCurrent() const
{
    if (last_row == -1 || last_col == -1) return false;
    State turn = (curturn == B ? W : B);
    int row = last_row, col = last_col;
    // horizontal
    int tot = 1;
    for (int i = 1; tot < 5 && col+i <= cols-1 && states[row][col+i]==turn;i++,tot++);
    for (int i = -1;tot < 5 && col+i >= 0 && states[row][col+i]==turn;i--,tot++);
    if (tot >= 5) return true;
    // verticle
    tot = 1;
    for (int i = 1;tot < 5 && row+i <= rows-1 && states[row+i][col]==turn;i++,tot++);
    for (int i = -1;tot < 5 && row+i >= 0 && states[row+i][col]==turn;i--,tot++);
    if (tot >= 5) return true;
    // main cross
    tot = 1;
    for (int i = 1;
    tot < 5 && 
    row+i <= rows-1 && col+i <= cols-1 &&
    states[row+i][col+i]==turn;
    i++,tot++);
    for (int i = -1;
    tot < 5 && 
    row+i >= 0 && col+i >=0 &&
    states[row+i][col+i]==turn;
    i--,tot++);
    if (tot >= 5) return true;
    // other cross
    tot = 1;
    for (int i = 1;
    tot < 5 && 
    row+i <= rows-1 && col-i >= 0 &&
    states[row+i][col-i]==turn;i++,tot++);
    for (int i = -1;
    tot < 5 && 
    row+i >= 0 && col-i <= cols-1 &&
    states[row+i][col-i]==turn;i--,tot++);
    if (tot >= 5) return true;
    return false;
}
