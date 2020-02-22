#ifndef _GAME_H
#define _GAME_H
#include <map>
#include <functional>
#include <memory>
#include "board.h"
using EventHandler = std::function<bool(chtype)>;
class JudgeStrategy
{
public:
    JudgeStrategy(const Board& _board):board(_board){}
    // true for find 5
    virtual bool judgeAll(State turn) const{ return false;}
    virtual bool judgeAll(State turn, vector<Pos>& win_pos) {return false;}
    virtual bool judgeCurrent(Pos cur_pos) const;
    virtual bool judgeCurrent(Pos cur_pos, vector<Pos>& win_pos) const;
private:
    const Board& board;
};

class Controller
{
public:
    std::map<chtype, EventHandler> mp_handler; //< key map
    Controller(Board* board, BoardRenderer* renderer=nullptr);
    void setRenderer(BoardRenderer* _renderer){
        renderer = _renderer;
    }
    virtual bool handle(chtype ch);
protected:
    Board* board;
    BoardRenderer* renderer;
private:
    // handlers
    bool handleDirect(chtype ch);
    bool handleEnter(chtype ch);
    bool handleExit(chtype ch);

    Pos cursor;
};

/* Singleton */
class GobangGame
{
public:
    enum GameState{None=0, BWin=1, WWin=2, Draw=3};
    void setJudge(JudgeStrategy* new_judge){
        judge = new_judge;
    }
    void setController(Controller* new_controller){
        controller = new_controller;
        if (controller) controller->setRenderer(renderer);
    }
    void setRenderer(BoardRenderer* new_renderer){
        renderer = new_renderer;
        if (controller) controller->setRenderer(renderer);
    }
    void setBoard(Board* new_board){
        board = new_board;
    }
    static GobangGame* getInstance(){
        return instance;
    }
    void run();
    bool moveAt(Pos pos);
    void initCurses();
    State getTurn(){
        return turn;
    }
    const Board* getBoard() const{ return board; }
private:
    GobangGame();
    ~GobangGame(); // forbidden
    GobangGame(const GobangGame& ); // forbidden
    void operator=(const GobangGame&); // forbidden

    static GobangGame* instance;

    BoardRenderer* renderer;
    Controller* controller;
    JudgeStrategy* judge;
    Board* board;
    State turn;
    int rows, cols;
    GameState state;
    vector<Pos> win_pos;
};

#endif