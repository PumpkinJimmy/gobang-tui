#ifndef __BOARD_H
#define __BOARD_H
#include <ncurses.h>
WINDOW* createBoard(int, int, int, int);
class BoardWin
{
public:
    enum Color {BLACK, WHITE};
    BoardWin(int rows_, int cols_, int top_, int left_);

    void setAt(int row, int col, Color color);

    ~BoardWin();

private:
    int rows;
    int cols;
    int top;
    int left;
    int width;
    int height;
    int bcolor;
    int wcolor;
    WINDOW* win;
    void drawBoard();
    BoardWin(const BoardWin& other); //forbidden
    BoardWin& operator=(const BoardWin& other); //forbidden
};
#endif