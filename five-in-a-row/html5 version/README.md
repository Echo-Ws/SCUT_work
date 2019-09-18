# Five-in-a-row

## File description:

1. chessboard.js:  
   All the action related to chessboard, such as draw the chessboard, use two-dimensional array to save the status of chesses, etc.  
    负责绘制棋盘，存储棋盘等一切和棋盘相关的操作。

2. const.js  
   Definition of all the const, such as the chess margin, edge number, etc.  
   定义了所有要用的常量，比如棋盘距离，大小，各类棋形的评分

3. evaluate.js  
   负责计算最佳落棋地点。主要实现了统计棋形，计算得分，alphabeta搜索的功能。

4. interact.js  
   Take charge of interaction from player to Ai. All the events are controlled by this file. Use Web worker to solve the unresponsive page when the AI script running.
   描述整个下棋的交互过程，相当于c++中的main函数。为了解决在启发式搜索时出现的页面卡顿，引入了htm5的特性web worker。在运行时要注意使用支持该特性的浏览器。而该项目的使用方式调用了本地的文件，由于安全原因在chrome上会出现cannot be accessed from origin ‘null’的问题，可以搭建本地服务器，然后通过本地服务器访问
