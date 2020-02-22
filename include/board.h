#ifndef __BOARD_H
#define __BOARD_H
#include <ncurses.h>
#include <vector>
#include <array>
#define COLOR_INVIS -1
using std::vector;
using Pos = std::array<int, 2>;
enum class State {E=0, B=1, W=2, X=3}; //B:Black; W:White; X:Edge;
using BoardMat = vector<vector<State>>;

class BoardWin
{
public:
    BoardWin(int rows_, int cols_, int top_, int left_);

    void setAt(int row, int col, int color);
    void clearAt(int row, int col);
    void setAt(int row, int col, chtype ch, attr_t attr, int fgcolor, int _bgcolor=-1);

    ~BoardWin();

private:
    int rows;
    int cols;
    int top;
    int left;
    int width;
    int height;
    int bgcolor;
    int cpairs[8];
    int cid_cnt;
    WINDOW* win;
    void drawBoard();
    BoardWin(const BoardWin& other); //forbidden
    BoardWin& operator=(const BoardWin& other); //forbidden
};
/*
 * Chessboard abstraction
 * Index from 1
 */
class Board
{
public:
    Board(int rows_, int cols_);
    const BoardMat& getMat() const;
    State getAt(Pos pos) const;
    bool setAt(Pos pos, State turn);
    // getters
    int getRows() const{
        return rows;
    }
    int getCols() const{
        return cols;
    }

private:
    int rows;
    int cols;
    BoardMat states;
};

class BoardRenderer
{
public:
    BoardRenderer(BoardWin* bwin, const Board* board);
    void renderChess(Pos chess_pos);
    void renderChessAll();
    void renderCursor(Pos pos);
    void renderBlink(vector<Pos> pos);
private:
    BoardWin* bwin;
    const Board* board;
    Pos last_cursor;
    int rows, cols;
};
#endif