#include "search.h"

 //初始化(棋盘等)              
void brain_init(){
	pipeOut("DEBUG brain_init");
	if(width<5 || width>MAX_BOARD || height<5 || height>MAX_BOARD){
    pipeOut("ERROR size of the board");
    return;
  }
  
  //初始化棋盘
  boardInit();
  evaluationInit(width,height);
  initHistoryTable(width,height);
  pipeOut("OK");
}

//开始
void brain_restart()
{
	pipeOut("DEBUG brain_restart");
	boardInit();
	evaluationInit(width,height);
	initHistoryTable(width,height);
	pipeOut("OK");
}


int isFree(int x, int y)
{
	return x>=0 && y>=0 && x<width && y<height && Square(x,y)->z == EMPTY_MOVE;
}
//AI 下子 检测下子位置是否合法
void brain_my(int x,int y)
{
	pipeOut("DEBUG brain_my [%d,%d]",x,y);
  if(isFree(x,y)){
	  SetChessOnBoard(x,y,TURN_MOVE);
  }else{
    pipeOut("ERROR my move [%d,%d]",x,y);
  }
}

//对手下子，同时检查是否合法
void brain_opponents(int x,int y) 
{
	pipeOut("DEBUG brain_opponents [%d , %d]",x,y);
  if(isFree(x,y)){
	  SetChessOnBoard(x,y,OPPONENT_MOVE);
  }else{
    pipeOut("ERROR opponents's move [%d,%d]",x,y);
  }
}

//棋盘外？？
void brain_block(int x,int y)
{
  if(isFree(x,y)){
	  SetChessOnBoard(x,y,OUTSIDE_MOVE);
  }else{
    pipeOut("ERROR winning move [%d,%d]",x,y);
  }
}

//undo
int brain_takeback(int x,int y)
{
	pipeOut("DEBUG brain_takeback [%d , %d]" ,x, y);
	if(!isFree(x,y)){
	  SetChessOnBoard(x,y,EMPTY_MOVE);
    return 0;
  }
  return 2;
}



#define MATCH_SPARE 7      //how much is time spared for the rest of game

//算法停止时间
DWORD stopTime()
{
	return start_time + min(info_timeout_turn, info_time_left/MATCH_SPARE)-30;
}


//计算下棋 !!
void brain_turn() 
{ 
	pipeOut("DEBUG brain_turn");

	//使用negamax+迭代加深
	IterDeeping();
	//使用简单搜索，固定深度
	//SimpleSearch();
}

void brain_end()
{
	freeResource();
}


//这里未实现
#ifdef DEBUG_EVAL
#include <windows.h>

void brain_eval(int x,int y)
{
  HDC dc;
  HWND wnd;
  RECT rc;
  char c;
  wnd=GetForegroundWindow();
  dc= GetDC(wnd);
  GetClientRect(wnd,&rc);
  c=(char)(board[x][y]+'0');
  TextOut(dc, rc.right-15, 3, &c, 1);
  ReleaseDC(wnd,dc);
}

#endif

//计算总迭代数和剪枝数
int count = 0, beta_count = 0;
static bool time_out = 0;



//迭代加深搜索+NegaMax/alphaBeta搜索 
void IterDeeping()
{
	//分配搜索时间   
    int nMinTimer =  GetTickCount() + unsigned int((min(info_timeout_turn, info_time_left/MATCH_SPARE)-30)*0.618f);   
	reSetHistoryTable(width,height);

	//存历史成绩 因为深度越低算出来的分数越不靠谱，所以不能比较大小 只有当当前层没搜索完时才用
	//然而历史搜索每次会第一个搜索上次最佳的结点 所以没必要存？
	/*Mov hisMov;
	hisMov.val = -9999;*/
	int mindepth = 1;
	//迭代加深搜索 搜索的深度取决于时间 
    for(int depth= mindepth; depth<20;depth *=2)
    {   
		time_out = 0;
		pipeOut("DEBUG MAX depth:%d,",depth);
           
        int score;
		//负极大值搜索 or // alpha-beta搜索 对象是棋盘
		//这里的player 0自己 1敌人
		//score = NegaMax(depth,0,depth);    
		score = alphabeta(depth,-19999,19999,0,depth);                     
		pipeOut("DEBUG time out :%d", time_out);
		pipeOut("DEBUG score value:%d",score);
		pipeOut("DEBUG BestMove:[%d,%d],%d",bestMove.x,bestMove.y,bestMove.val);
		pipeOut("DEBUG Time use:%d,time left:%d",GetTickCount()- start_time,info_timeout_turn-(GetTickCount()-start_time));
		pipeOut("DEBUG 迭代次数：%d", count);
		pipeOut("DEBUG 减枝数：%d", beta_count);
		count = 0;
		beta_count = 0;
		if (time_out)
		{
			pipeOut("DEBUG 本次迭代未完成");
			/*if (hisMov.val != -9999 && depth != mindepth) {
				pipeOut("DEBUG 采用上次的迭代结果");
				bestMove = hisMov;
			}*/

			
		}

		// 若强行终止思考，停止搜索   
		if(terminate)   
            break;   
   
        // 若时间已经达到规定时间的一半，再搜索一层的时间可能不够，停止搜索。   
        if( GetTickCount() > nMinTimer)   
            break;   
   
        // 在规定的深度内，遇到杀棋，停止思考。   
		if( score >= 9999)   
              break;   
		
		//hisMov = bestMove;

		
    }   
	
	
	do_mymove(bestMove.x,bestMove.y);
}


static bool note = 0;

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
		/*if (depth == MaxDepth)
			pipeOut("DEBUG 读历史表:[%d,%d],%d", moveList[i].x, moveList[i].y, moveList[i].val);*/
	}
	


	moveList = MergeSort(moveList, moveListLen); //历史启发排序，alpha-beta时可用
	
	if (depth == MaxDepth) {

		pipeOut("DEBUG 第一个搜索点:[%d,%d],%d", moveList[0].x, moveList[0].y, moveList[0].val);
	}


	//if (note) {
	//	pipeOut("DEBUG 可下子个数:%d", moveListLen);
	//}

	
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
			return 9999+depth;
		}

		/*if (depth == MaxDepth)
		{
			if (moveList[i].x == 11 && moveList[i].y == 5)
			{
				pipeOut("DEBUG 进入目标点:[%d,%d],%d", moveList[i].x, moveList[i].y, moveList[i].val);
				note = 1;
			}

		}*/
		
		
		
		moveList[i].val = -alphabeta(depth - 1, -beta, -alpha, 1 - player, MaxDepth);

		

		/*if (depth == MaxDepth)
		{
			if (moveList[i].x == 11 && moveList[i].y == 5)
			{
				pipeOut("DEBUG 获得分数:[%d,%d],%d", moveList[i].x, moveList[i].y, moveList[i].val);
				note = 0;
			}
		}
		
		if (note) {
			pipeOut("DEBUG 在目标点中的点:[%d,%d],%d", moveList[i].x, moveList[i].y, moveList[i].val);

		}*/
		UnmakeMove(moveList[i]);
		
		//时间超时 可能没搜完全部可行子 结果不可信
		if (time_out)
		{
			break;
		}

		if (alpha < moveList[i].val)
		{
			if (depth == MaxDepth)
			{
				pipeOut("DEBUG 当前是最顶层！！！！！！！！");
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
	/*	if(moveList[bestMoveIndex].val > bestMove.val)
			pipeOut("DEBUG 写历史表:[%d,%d],%d", moveList[bestMoveIndex].x, moveList[bestMoveIndex].y, moveList[bestMoveIndex].val);*/
		enterHistoryScore(moveList[bestMoveIndex], depth, player);
	}

	if (depth == MaxDepth && bestMoveIndex!=-1 && time_out!=1)
	{		
		bestMove = moveList[bestMoveIndex];	
	}
	delete[] moveList;
	moveList = NULL;
	return alpha;
}

//产生当前棋盘的可行棋步，只获取棋盘上已有棋子2字以内的空棋位，并且考虑是否禁手
Mov* GenerateMoves(int& moveLen,int player)
{
	int range = 2;
	int count = 0;
	Psquare p = boardb;

	Mov* result = new Mov[width*height];

	//初始化标志位，避免重复加入最后队列
	int* makeSign = new int[width*height];
	for (int i = 0; i < width; i++)
	{
		for (int j = 0; j < height; j++)
		{
			makeSign[i*width+j] = 0;
		}
	}

	//开始获取可行的棋位
	while(p != boardk)
	{
		if (count >= 20)range = 1;

		//如果有子
		if (p->z == TURN_MOVE || p->z == OPPONENT_MOVE)
		{
			for(int i = ( p->x - range < 0 ? 0 : p->x - range);i <= ( p->x + range > width ? width : p->x + range); i++)
			{
				for (int j = ( p->y - range <0 ? 0: p->y - range); j <= (p->y + range > height ? height:p->y + range); j++)
				{
					
					if ( Square(i,j)->z == EMPTY_MOVE && makeSign[i*width+j] == 0)
					{
						makeSign[i*width+j]=1;
						//禁手判断，函数里会首先判断该选手是否需要禁手判断，在继续相关操作
						if (isBan(Square(i,j),player))
						{
							continue;
						}
						result[count].x = i;
						result[count].y = j;
						result[count].val = 0;
						count++;
 					}
				}
			}	
		}
		//?为什么要转char*在加 
		//移向下一个位置
		p = (Psquare)(((char*)p)+ sizeof(Tsquare));
	}
	
	delete[] makeSign;
	//棋盘为空，选择中心点 1.4s
	if (count == 0 )//棋盘为空或者棋盘已满
	{
		if (Square(0, 0)->z == EMPTY_MOVE) {
			srand(GetTickCount());					
			//选择中心点
			int rndX = width / 2 - 1;
			int rndY = height / 2 - 1;
			
			result[count].x = rndX;
			result[count].y = rndX;
			result[count].val = 0;	
			count++;
		}
		
	}
	
	moveLen = count;
	return result;
}


//
Mov mov_rd(Mov* moveList,int count)
{
	srand(GetTickCount());
	int num = rand()%count;
	//pipeOut("DEBUG rnd :%d",num);
	return moveList[num];
}



//判断是否是禁手关键点
bool isBan(Psquare p0,int player)
{
	//检查禁手
	if (info_fb_check && player == firstPlayer )
	{
		ChessAnalyzeData checkData[4];
		p0->z = player+1;
		int fb_type = ForbiddenCheck(checkData,p0);
		p0->z =  EMPTY_MOVE;
		
		if(fb_type != NO_FORBIDDEN)
		{
			return true;
		}
	}
	return false;
}



//判断执行move后棋局是否结束
bool isGameOver(Mov move)
{
	Psquare p0 = Square(move.x,move.y);
	
	for(int i=0;i<4;i++)
	{
		int s = diroff[i];
		Psquare pPrv = p0;
		Psquare pNxt = p0;
		int count = -1;
		do
		{
			prvP(pPrv,1);
			count++;
		}while(pPrv->z == p0->z);
		do
		{
			nxtP(pNxt,1);
			count++;
		} while (pNxt->z == p0->z);
		
		if (count == 5)
			return true;
		else if(count > 5 && !info_exact5 )
			return true;
	}
	return false;
}



void freeResource()
{
	delete []PosValue;
	PosValue = NULL;
	
	delete []m_nRecord;
	m_nRecord = NULL;
};

//负极大值搜索+历史表启发
int NegaMax(int depth, int player, int MaxDepth)
{

	if (depth <= 0)//预判层上的伪叶子结点，计算其启发评价值 
	{
		return evaluate(player);
	}

	int bestMoveIndex = -1;
	int bestVal = -10000;
	int moveListLen = 0;
	//可能下子的位置
	Mov* moveList = GenerateMoves(moveListLen, player);

	if (moveListLen == 0)
	{
		pipeOut("gen movelist is empty");
		delete[] moveList;
		moveList = NULL;
		return evaluate(player);
	}

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
			break;
		}

		MakeMove(moveList[i], player);

		//儿子结点为胜负已分状态（真正的叶子结点），表明player方走这一步走法后会获胜
		if (isGameOver(moveList[i]))
		{
			if (depth == MaxDepth)
			{
				bestMove = moveList[i];
			}
			UnmakeMove(moveList[i]);
			delete[] moveList;
			moveList = NULL;
			return 9999;
		}

		moveList[i].val = -NegaMax(depth - 1, 1 - player, MaxDepth);


		UnmakeMove(moveList[i]);

		if (bestVal < moveList[i].val)
		{

			bestVal = moveList[i].val;
			bestMoveIndex = i;
			if (depth == MaxDepth)
			{
				bestMove = moveList[i];
			}
		}

	}

	if (bestMoveIndex != -1)
	{
		enterHistoryScore(moveList[bestMoveIndex], depth, player);
	}

	if (depth == MaxDepth)
	{
		bestMove = moveList[bestMoveIndex];
	}
	delete[] moveList;
	moveList = NULL;

	return bestVal;
}

//固定深度+NegaMax搜索 
void SimpleSearch()
{
	Mov resultMov;
	int depth = 4;

	bestMove.val = -10000;
	int score;
	//负极大值搜索
	score = NegaMax00(depth, 0, depth);
	pipeOut("DEBUG Depth value:%d", score);
	pipeOut("DEBUG BestMove:[%d,%d],%d", bestMove.x, bestMove.y, bestMove.val);
	do_mymove(bestMove.x, bestMove.y);
}

//负极大值搜索
int NegaMax00(int depth, int player, int MaxDepth)
{
	if (depth <= 0)//伪叶子结点
	{
		return evaluate(player);
	}

	int bestMoveIndex = -1;
	int bestVal = -10000;
	int moveListLen = 0;
	Mov* moveList = GenerateMoves(moveListLen, player);

	if (moveListLen == 0)
	{
		pipeOut("gen movelist is empty");
		delete[] moveList;
		moveList = NULL;
		return evaluate(player);
	}

	for (int i = 0; i<moveListLen; i++)
	{
		if (terminate || GetTickCount() >= stopTime())
		{
			pipeOut("DEBUG It's time to terminate");
			break;
		}

		MakeMove(moveList[i], player);

		if (isGameOver(moveList[i]))//儿子结点为胜负已分状态，真正的叶子结点
		{
			if (depth == MaxDepth)
			{
				bestMove = moveList[i];
			}
			UnmakeMove(moveList[i]);
			delete[] moveList;
			moveList = NULL;
			return 9999;
		}

		moveList[i].val = -NegaMax(depth - 1, 1 - player, MaxDepth);

		UnmakeMove(moveList[i]);

		if (bestVal < moveList[i].val)
		{

			bestVal = moveList[i].val;
			bestMoveIndex = i;
			if (depth == MaxDepth)
			{
				bestMove = moveList[i];
			}
		}

	}

	if (depth == MaxDepth)
	{
		bestMove = moveList[bestMoveIndex];
	}
	delete[] moveList;
	moveList = NULL;

	return bestVal;
}
