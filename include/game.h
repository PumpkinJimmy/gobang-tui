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
    void init(JudgeStrategy* _judge, BoardRenderer* _renderer, Board* _board, Controller* _controller);
    static GobangGame* getInstance(){
        return instance;
    }
    void run();
    bool moveAt(Pos pos);
    static void initCurses();
    State getTurn(){
        return turn;
    }
    const Board* getBoard() const{ return board; }
private:
    GobangGame();
    ~GobangGame(); // forbidden
    GobangGame(const GobangGame& ); // forbidden
    void operator=(const GobangGame&); // forbidden

    static bool has_init;
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

class GobangGameFactory
{
public:
    GobangGameFactory(int _rows, int _cols, int top, int left);
    virtual void createBoard();
    virtual void createBoardWin();
    virtual void createRenderer();
    virtual void createController();
    virtual void createJudgeStrategy();
    int getRows() const { return m_rows; }
    int getCols() const { return m_cols; }
    GobangGame* initGobangGame();
    ~GobangGameFactory();
protected:
    Board* m_board;
    BoardWin* m_bwin;
    BoardRenderer* m_renderer;
    JudgeStrategy* m_judge;
    Controller* m_controller;
    GobangGame* m_game;
    int m_rows;
    int m_cols;
    int m_top;
    int m_left;
};
#endif