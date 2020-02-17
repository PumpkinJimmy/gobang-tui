## TODO List
- 改进CMakeList，使debug跟release分离处理
- 把游戏流程逻辑分离出来，把控制逻辑分离出来（要点：Board只提供对棋盘的底层抽象，通过其他类访问BoardWin来提供其他功能）
- 把渲染逻辑分离出来(BoardRender)（用以解决上层类与BoardWin的交互）
- 尝试用shared_ptr管理
- 计算机下棋