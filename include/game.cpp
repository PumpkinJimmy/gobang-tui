#ifndef _GAME_H
#define _GAME_H
#include <map>
#include <functional>
#include "board.h"
using EventHandler = std::function<bool(void)>;
class JudgeStrategy
{
public:
    virtual bool judgeAll(const Mat& mat, int val);
    virtual bool judgeAll(const Mat& mat, int val, vector<Pos>& win_pos);
    virtual bool judgeCurrent(const Mat& mat, Pos cur_pos);
    virtual bool judgeCurrent(const Mat& mat, Pos cur_pos, vector<Pos>& win_pos);
};

class Controller
{
public:
    static std::map<chtype, EventHandler> mp_handler; //< key map
    Controller(Board* board, BoardRenderer* renderer);
    virtual bool handle(chtype ch);
    bool handleLeft();
    bool handleRight();
    bool handleUp();
    bool handleDown();
};

/* Singleton */
class GobangGame
{
public:
    void setJudge(JudgeStrategy* new_judge);
    void setController(Controller* new_controller);
    static GobangGame* getInstance();
    void run();
private:
    GobangGame();
    ~GobangGame(); // forbidden
    GobangGame(const GobangGame& ); // forbidden
    void operator=(const GobangGame&); // forbidden

    static GobangGame* instance;

    Controller* controller;
    JudgeStrategy* judge;
    
};

#endif