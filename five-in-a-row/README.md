临近毕业，着手整理本科四年的代码。本篇整理的是于去年大三完成的五子棋代码。  
课程需求完成基于c++对战平台的pbrain.exe 的AI开发。详细的课程要求和完成情况可以见c++ version中的五子棋实验报告。




主要实现的启发式搜索函数：
```C++
//alphaBeta搜索负极大值形式
int alphabeta(int depth,int alpha,int beta,int player,int MaxDepth)
{
	count++;
	if (depth <= 0)//预判层上的伪叶子结点，计算其启发评价值 
	{
		return evaluate(player);
	}

	int bestMoveIndex = -1;
	int moveListLen = 0;

	//可能下子的位置
	Mov* moveList = GenerateMoves(moveListLen, player);
	//无棋时下中间
	if (moveListLen == 1) {
		bestMove = moveList[0];
		return 9999;
	}
	
	for (int i = 0; i < moveListLen; i++)
	{
		moveList[i].val = getHistoryScore(moveList[i], player);
	}
	


	moveList = MergeSort(moveList, moveListLen); //历史启发排序，alpha-beta时可用

	
    //向下搜索
	for (int i = 0; i<moveListLen; i++)
	{
		if (terminate || GetTickCount() >= stopTime())
		{
			pipeOut("DEBUG It's time to terminate");
			time_out = 1;
			break;
		}

		MakeMove(moveList[i], player);

		//儿子结点为胜负已分状态（真正的叶子结点），表明player方走这一步走法后会获胜
		if (isGameOver(moveList[i]))
		{
			//-1：在下一层遇到说明是必须守之棋
			if (depth == MaxDepth )//|| depth == MaxDepth-1)
			{
				bestMove = moveList[i];
			}
			UnmakeMove(moveList[i]);
			delete[] moveList;
			moveList = NULL;
			//负极大值是否要考虑是谁赢 从而返回+- :不用 一定是当前下子方连5子。否则就结束了
			return 9999;
		}

		moveList[i].val = -alphabeta(depth - 1, -beta, -alpha, 1 - player, MaxDepth);
		if (depth == MaxDepth)
		{
			if (moveList[i].x == 5 && moveList[i].y == 8)
			{
				pipeOut("DEBUG 获得分数:[%d,%d],%d", moveList[i].x, moveList[i].y, moveList[i].val);
				note = 0;
			}

		}
	/*	if (note) {
			pipeOut("DEBUG 在目标点中的点:[%d,%d],%d", moveList[i].x, moveList[i].y, moveList[i].val);

		}*/
		UnmakeMove(moveList[i]);
		

		if (alpha < moveList[i].val)
		{
			if (depth == MaxDepth)
			{
				pipeOut("DEBUG depth = Maxdepth");
				pipeOut("DEBUG 当前阿尔法比较大的点:[%d,%d],%d", moveList[i].x, moveList[i].y, moveList[i].val);
				pipeOut("DEBUG 之前alpha value:%d", alpha);
			}
				
			alpha = moveList[i].val;
			bestMoveIndex = i;
			if (depth == MaxDepth)
			{
				bestMove = moveList[i];
			}
		}
		if (alpha >= beta) {
			beta_count++;
			break;// 剪枝
		}

	}

	if (bestMoveIndex != -1)
	{
		enterHistoryScore(moveList[bestMoveIndex], depth, player);
	}

	if (depth == MaxDepth && bestMoveIndex!=-1)
	{		
		bestMove = moveList[bestMoveIndex];	
	}
	delete[] moveList;
	moveList = NULL;
	return alpha;
}

```
接下来把c++版本的五子棋重构成基于html5的五子棋代码。  

首先在const.js 定义了所有要用的常量，比如棋盘距离，大小，各类棋形的评分。  

chessboard.js 负责绘制棋盘，存储棋盘等一切和棋盘相关的操作。  

interact.js描述整个下棋的交互过程，相当于c++中的main函数。为了解决在启发式搜索时出现的页面卡顿，引入了htm5的特性web worker。在运行时要注意使用支持该特性的浏览器。而该项目的使用方式调用了本地的文件，由于安全原因在chrome上会出现*cannot be accessed from origin ‘null’*的问题，可以搭建本地服务器，然后通过本地服务器访问；或者使用Firefox解决。   

核心的预测函数放在evaluate.js中实现。包括统计棋形，计算得分，alphabeta搜索。基本思路和c++ version相似，可以说是将c++翻译成js罢了。

总结一下，本项目中的五子棋AI只利用了alphabeta搜索，应付博主这种业余水平足够，但改进空间很大，可以参考[五子棋AI算法8篇](https://blog.csdn.net/lihongxun945/article/category/6089493)。








