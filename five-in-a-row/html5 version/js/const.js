/**
 * Created by Administrator on 2016/11/16.
 */


//格子之间的间隔
const INTERVAL =40;
//棋盘边距离
const MARGIN = 20;
//边数 棋盘用15*15
const EDGR_NUM = 15;

//define chess type
const BLACK = 1;
const WHITE = -1;
const EMPTY = 0;

//define direction
const LH = 0; //左横
const RH = 1; //右横
const TOP = 2;//上
const DOWN = 3;//下
const LLS = 4;// left slash
const RLS = 5;
const LRS = 6;//right slash
const RRS = 7;

const TWO = 0;//活二
const THREE = 1;//活三
const FOUR = 2;//活四
const STWO = 3;//冲二
const STHREE = 4;//冲三
const SFOUR = 5;//冲四

const DEBUG = 0;