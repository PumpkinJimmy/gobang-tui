#ifndef __BOARD_H
#define __BOARD_H
#include <ncurses.h>
#include <vector>
#include <array>
#define COLOR_INVIS -1
using std::vector;
using Pos = std::array<int, 2>;
using Mat = vector<vector<int>>;
enum State{B=1, W=2};
enum {E=0, SE=3, ISE=4};
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
class Board
{
public:
    Board(int rows_, int cols_, BoardWin* win);
    const Mat& getMat() const;
    bool setAt(int row, int col, State turn);
    // for controller
    bool handle(chtype key);
    // getters
    int getRows() const;
    int getCols() const;
    // State getTurn() const;
    // render
    void render();
    // judge
    bool judgeCurrent(int res_chess[5][2]) const;
    bool judgeCurrent() const;
    bool judgeAll(int res_chess[5][2]) const;
    bool isWin() const { return is_win; }

private:
    // state
    int rows;
    int cols;
    Mat states;
    State curturn;
    // controlller
    int currow;
    int curcol;
    // render
    BoardWin* bwin;
    const int cid[5] = {COLOR_BLUE, COLOR_BLACK, COLOR_WHITE, COLOR_YELLOW, COLOR_RED};   
    void render_cursor(int row, int col);
    // judge
    int last_row, last_col;
    // game
    bool is_win;
};

class BoardRenderer
{
public:
    BoardRenderer(BoardWin* bwin, const Board* board);
    void renderChess();
    void renderCursor(Pos pos);
    void renderBlink(vector<Pos> pos);
private:
    BoardWin* bwin;
    const Board* board;
};
#endif