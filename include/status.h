#ifndef __STATUS_H
#define __STATUS_H
#include <ncurses.h>
class StatusBar
{
public:
    void setText(const char* str);
    void setText(char ch);
    static StatusBar* getInstance();
    ~StatusBar();
private:
    WINDOW* win;
    static StatusBar* instance;
    StatusBar();
    StatusBar(const StatusBar& other);
    StatusBar& operator=(const StatusBar& other);
};

#endif