#ifndef __BOARD_H
#define __BOARD_H
#include <ncurses.h>
#include <vector>
#define COLOR_INVIS -1
using std::vector;
WINDOW* createBoard(int, int, int, int);
class BoardWin
{
public:
    BoardWin(int rows_, int cols_, int top_, int left_);

    void setAt(int row, int col, int color);

    ~BoardWin();

private:
    int rows;
    int cols;
    int top;
    int left;
    int width;
    int height;
    int bgid;
    int cpairs[8];
    WINDOW* win;
    void drawBoard();
    BoardWin(const BoardWin& other); //forbidden
    BoardWin& operator=(const BoardWin& other); //forbidden
};
class Board
{
public:
    enum State{E=0, B=1, W=2};
    typedef vector<vector<int>> Mat;
    Board(int rows_, int cols_, BoardWin* win);
    const Mat& getMat() const;
    bool setAt(int row, int col, State turn);
    int getRows() const;
    int getCols() const;
private:
    int rows;
    int cols;
    BoardWin* bwin;
    Mat states;
};
#endif