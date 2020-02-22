#include <ncurses.h>
#include <cstdlib>
#include "board.h"
#include "status.h"

BoardWin::BoardWin(int rows_, int cols_, int top_, int left_) : 
top(top_), left(left_), rows(rows_), cols(cols_), cid_cnt(10), bgcolor(COLOR_BLUE)
{
    height = rows * 2 + 1;
    width = cols * 3 + 1;
    for (int i = 0; i < 8; i++)
    {
        init_pair(i + 1, COLOR_BLUE, i);
        cpairs[i] = i + 1;
    }
    win = newwin(height, width, top, left);
    drawBoard();
}
void BoardWin::setAt(int row, int col, chtype ch, attr_t attr, int fgcolor, int _bgcolor)
{
    if (row < 0 || row >= rows || col < 0 || col >= cols)
        throw "Try to render chess at invalid position";
    int grow = row * 2 + 1, gcol = col * 3 + 1;
    if (_bgcolor == -1) _bgcolor = bgcolor;
    mvwaddch(win, grow, gcol, ch);
    mvwaddch(win, grow, gcol+1, ch);
    init_pair(++cid_cnt, fgcolor, _bgcolor);
    // init_pair(++cid_cnt, COLOR_WHITE, COLOR_BLUE);
    mvwchgat(win, grow, gcol, 2, attr, cid_cnt, NULL);
    // mvwchgat(win, grow, gcol, 2, A_BLINK, cid_cnt, NULL);
    wrefresh(win);
}
void BoardWin::setAt(int row, int col, int color)
{
    if (row < 0 || row >= rows || col < 0 || col >= cols)
        throw "Try to render chess at invalid position";
    int grow = row * 2 + 1, gcol = col * 3 + 1;
    int cid;
    if (color == -1) cid = 0;
    else cid = cpairs[color];
    mvwaddch(win, grow, gcol, ' ');
    mvwaddch(win, grow, gcol+1, ' ');
    mvwchgat(win, grow, gcol, 2, A_NORMAL, cid, NULL);
    wrefresh(win);
}
void BoardWin::clearAt(int row, int col)
{
    setAt(row, col, -1);
}
BoardWin::~BoardWin()
{
    delwin(win);
}
void BoardWin::drawBoard()
{
    attron(COLOR_PAIR(0));
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
    attroff(COLOR_PAIR(0));
    wrefresh(win);
}
Board::Board(int rows_, int cols_)
:rows(rows_), cols(cols_)
{
    states.resize(rows+2);
    for (auto &st: states[0]){
        st = State::X;
    }
    for (auto &st: states[cols+1]){
        st = State::X;
    }
    for (auto &vec : states){
        vec.resize(cols+2);
        vec[0] = State::X;
        vec[cols+1] = State::X;
    }
}
const BoardMat& Board::getMat() const
{
    return states;
}
State Board::getAt(Pos pos) const
{
    return states[pos[0]][pos[1]];
}
bool Board::setAt(Pos pos, State turn)
{
    if (turn != State::B && turn != State::W)
    {
        throw "Invalid state set";
    }
    int row = pos[0], col = pos[1];
    if (row < 0 || row > rows - 1 ||
        col < 0 || col > cols - 1)
        throw "Pos Set Overflow";
    else if (states[row][col] != State::E)
        return false;
    else
        states[row][col] = turn;
        return true;
}
BoardRenderer::BoardRenderer(BoardWin* _bwin, const Board* _board):
bwin(_bwin), board(_board), last_cursor({1, 1})
{
    rows = board->getRows();
    cols = board->getCols();
}
void BoardRenderer::renderChessAll()
{
    const BoardMat& mat = board->getMat();
    for (int i = 1; i <= rows; i++){
        for (int j = 1; j <= cols; j++){
            if (mat[i][j] == State::B || mat[i][j] == State::W){
                int color = 
                    (mat[i][j] == State::B ? COLOR_BLACK : COLOR_WHITE);
                bwin->setAt(i-1, j-1, color);
            }
        }
    }
}
void BoardRenderer::renderChess(Pos chess_pos)
{
    State st = board->getAt(chess_pos);
    if (st == State::B || st == State::W){
        int color = (st == State::B ? COLOR_BLACK : COLOR_WHITE);
        bwin->setAt(chess_pos[0]-1, chess_pos[1]-1, color);
    }
}
void BoardRenderer::renderCursor(Pos pos)
{
    bwin->clearAt(last_cursor[0]-1, last_cursor[1]-1);
    renderChess(last_cursor);
    State st = board->getAt(pos);
    if (st == State::E)
        bwin->setAt(pos[0]-1, pos[1]-1, COLOR_YELLOW);
    else
        bwin->setAt(pos[0]-1, pos[1]-1, COLOR_RED);
    last_cursor = pos;
}
void BoardRenderer::renderBlink(vector<Pos> pos)
{
    for (auto& p: pos){
        State st = board->getAt(p);
        if (st == State::B)
            bwin->setAt(p[0]-1, p[1]-1, '#', A_BLINK, COLOR_BLACK);
        else if (st == State::W)
            bwin->setAt(p[0]-1, p[1]-1, '#', A_BLINK, COLOR_WHITE);
    }
}
