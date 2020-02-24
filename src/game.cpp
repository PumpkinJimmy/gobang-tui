#include <map>
#include <ncurses.h>
#include <functional>
#include "status.h"
#include "game.h"
#define BIND(k, h) mp_handler.insert({k, bind(&Controller::h, this, _1)})
using namespace std;
using namespace placeholders;
bool JudgeStrategy::judgeCurrent(Pos cur_pos, vector<Pos>& win_pos) const
{
    win_pos.clear();
    win_pos.resize(5);
    State turn = board.getAt(cur_pos);
    int row = cur_pos[0], col = cur_pos[1];
    int rows = board.getRows(), cols = board.getCols();
    win_pos[0] = {row, col};
    // horizontal
    int tot = 1;

    for (int i = 1;
    tot < 5 && col+i <= cols && board.getAt({row, col+i})==turn;
    win_pos[tot]={row,col+i},tot++,i++);

    for (int i = -1;
    tot < 5 && col+i >= 1 &&  board.getAt({row, col+i})==turn;
    win_pos[tot]={row,col+i},tot++,i--);

    if (tot >= 5) return true;

    // verticle
    tot = 1;

    for (int i = 1;
    tot < 5 && row+i <= rows && board.getAt({row+i,col})==turn;
    win_pos[tot]={row+i,col},tot++,i++);
    for (int i = -1;
    tot < 5 && row+i >= 1 && board.getAt({row+i,col})==turn;
    win_pos[tot]={row+i,col},tot++,i--);

    if (tot >= 5) return true;

    // main cross
    tot = 1;

    for (int i = 1;
    tot < 5 && 
    row+i <= rows && col+i <= cols &&
    board.getAt({row+i,col+i})==turn;
    win_pos[tot]={row+i,col+i},tot++,i++);

    for (int i = -1;
    tot < 5 && 
    row+i >= 1 && col+i >=1 &&
    board.getAt({row+i,col+i})==turn;
    win_pos[tot]={row+i,col+i},tot++,i--);

    if (tot >= 5) return true;

    // other cross
    tot = 1;

    for (int i = 1;
    tot < 5 && 
    row+i <= rows && col-i >= 1 &&
    board.getAt({row+i,col-i})==turn;
    win_pos[tot]={row+i,col-i},tot++,i++);

    for (int i = -1;
    tot < 5 && 
    row+i >= 1 && col-i <= cols &&
    board.getAt({row+i,col-i})==turn;
    win_pos[tot]={row+i,col-i},tot++,i--);

    if (tot >= 5) return true;

    return false;
}
bool JudgeStrategy::judgeCurrent(Pos cur_pos) const
{
    vector<Pos> tmp;
    return judgeCurrent(cur_pos, tmp);
}
Controller::Controller(Board *_board, BoardRenderer *_renderer) : board(_board), renderer(_renderer), cursor({1, 1})
{
    BIND(KEY_LEFT, handleDirect);
    BIND(KEY_RIGHT, handleDirect);
    BIND(KEY_UP, handleDirect);
    BIND(KEY_DOWN, handleDirect);
    BIND('\n', handleEnter);
    BIND(10, handleExit);
}
bool Controller::handle(chtype ch)
{
    if (!renderer)
        throw "Controller missing renderer";
    if (!mp_handler.count(ch))
    {
        return false;
    }
    else
        return mp_handler[ch](ch);
}

bool Controller::handleDirect(chtype ch)
{
    switch (ch)
    {
    case KEY_LEFT:
        if (cursor[1] > 1)
        {
            cursor[1]--;
            renderer->renderCursor(cursor);
        }
        return true;
    case KEY_RIGHT:
        if (cursor[1] < board->getCols())
        {
            cursor[1]++;
            renderer->renderCursor(cursor);
        }
        return true;
    case KEY_UP:
        if (cursor[0] > 1)
        {
            cursor[0]--;
            renderer->renderCursor(cursor);
        }
        return true;
    case KEY_DOWN:
        if (cursor[0] < board->getRows())
        {
            cursor[0]++;
            renderer->renderCursor(cursor);
        }
        return true;
    default:
        return false;
    }
}
bool Controller::handleEnter(chtype ch)
{
    if (GobangGame::getInstance()->moveAt(cursor))
    {
        renderer->renderChess(cursor);
        StatusBar::getInstance()->setText("Move Success");
    }
    else
        StatusBar::getInstance()->setText("Invalid move, try again.");
    return true;
}
bool Controller::handleExit(chtype ch)
{
    exit(0);
}
GobangGame* GobangGame::instance = new GobangGame;
bool GobangGame::has_init = false;
GobangGame::GobangGame():
turn(State::B), rows(0), cols(0), state(None)
{
    initCurses();
}
void GobangGame::init(JudgeStrategy* _judge, BoardRenderer* _renderer, Board* _board, Controller* _controller)
{
    judge = _judge;
    renderer = _renderer;
    board = _board;
    controller = _controller;
    controller->setRenderer(renderer);
    rows = board->getRows();
    cols = board->getCols();
    has_init = true;
}
GobangGame::~GobangGame(){
}
void GobangGame::initCurses(){
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    curs_set(0);
    start_color();
    init_color(COLOR_BLUE, 100, 100, 700);
    init_color(COLOR_BLACK, 0, 0, 0);
    assume_default_colors(COLOR_WHITE, COLOR_BLUE); // background color
}
void GobangGame::run(){
    if (!has_init)
        throw "GobangGame not initialized";
    renderer->renderCursor({1, 1});
    while (state == None)
    {
        chtype ch = getch();
        controller->handle(ch);
    }
    if (state == BWin){
        StatusBar::getInstance()->setText("Black Win the Game");
        renderer->renderBlink(win_pos);
    }
    else if (state == WWin){
        StatusBar::getInstance()->setText("White Win the Game");
        renderer->renderBlink(win_pos);
    }
    else if (state == Draw)
        StatusBar::getInstance()->setText("Draw");
    else
        throw "Stop for unknown reason";
}
bool GobangGame::moveAt(Pos pos)
{
    if (board->setAt(pos, turn))
    {
        if (judge->judgeCurrent(pos, win_pos)){
            state = (turn == State::B ? BWin : WWin);
            return true;
        }
        turn = (turn == State::B ? State::W : State::B);
        return true;
    }
    else return false;
}
GobangGameFactory::GobangGameFactory(int rows, int cols, int top, int left):
m_rows(rows), m_cols(cols), m_top(top), m_left(left){}
GobangGameFactory::~GobangGameFactory(){
    delete m_controller;
    delete m_judge;
    delete m_renderer;
    delete m_board;
    delete m_bwin;
}
GobangGame* GobangGameFactory::initGobangGame()
{
    createBoard();
    createBoardWin();
    createRenderer();
    createController();
    createJudgeStrategy();
    m_game = GobangGame::getInstance();
    m_game->init(m_judge, m_renderer, m_board, m_controller);
    return m_game;
}
void GobangGameFactory::createBoard()
{
    m_board = new Board(m_rows, m_cols);
}
void GobangGameFactory::createBoardWin()
{
    m_bwin = new BoardWin(m_rows, m_cols, m_top, m_left);
}
void GobangGameFactory::createRenderer()
{
    m_renderer = new BoardRenderer(m_bwin, m_board);
}
void GobangGameFactory::createController()
{
    m_controller = new Controller(m_board, m_renderer);
}
void GobangGameFactory::createJudgeStrategy()
{
    m_judge = new JudgeStrategy(*m_board);
}