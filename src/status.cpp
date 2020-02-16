#include "status.h"
StatusBar* StatusBar::instance = nullptr;
StatusBar* StatusBar::getInstance()
{
    if (instance == nullptr)
    {
        instance = new StatusBar();
    }
    return instance;
}
StatusBar::StatusBar()
{
    win = newwin(1, COLS, LINES-1, 0);
    wrefresh(win);
}
void StatusBar::setText(const char* str)
{
    wclrtoeol(win);
    mvwprintw(win, 0, 0, "%s", str);
    wrefresh(win);
}
void StatusBar::setText(char ch)
{
    wclrtoeol(win);
    mvwaddch(win, 0, 0, ch);
    wrefresh(win);
}
StatusBar::~StatusBar()
{
    delwin(win);
    delete instance;
}