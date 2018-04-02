
/**
 * Created by Administrator on 2016/11/9.
 */
importScripts('./const.js');
importScripts('./chessboard.js');


var ai = {
    init: function (type) {
        //记录ai所持棋色
        this.type =type;
    },
    think:function () {
        this.IterDeeping();


    },
    //统计棋型
    analysis: function () {
        var board =this.board;
        //记录不同玩家棋型数量 0 白 1黑
        var record = new Array(2);

        //初始化
        for (var i = 0; i < 2; i++) {
            record[i] = new Array(6);
            for (var j = 0; j < 6; j++) {
                record[i][j] = 0;
            }
        }


        //记录四个方向 是否有搜索过当前棋子
        var sets = new Array(4);
        for (var i = 0; i < 4; i++) {
            sets[i] = new Set();
        }


        for (var move of board.chesslist) {

            //记录两端被堵塞的个数
            var block = 0;
            //记录连子数
            var count = 1;

            //检查8个方向 同个方向每个子只需要检查一次
            for (var direction = 0; direction < 8; direction++) {
                var analyse_move = move;

                if (!(direction % 2) && sets[parseInt(direction / 2)].has(analyse_move.toString())) {
                    direction++;//跳过两个direction 即跳过一个方向
                    // console.log(analyse_move.toString()+"跳过")
                    continue;
                }


                for (var i = 1; i < 15; i++) {
                    //不成功则返回FALSE
                    // analyse_move.__proto__ = Move;
                    analyse_move = analyse_move.getNext(direction);
                    if (!analyse_move)
                    {
                        //没得下也相当于被堵住
                        block++;
                        break;
                    }
                    try {
                        var type = board.chessData[analyse_move.x][analyse_move.y];
                    }catch (err){
                        console.log(err)
                        console.log(direction)
                        console.log(analyse_move);
                    }
                    if (type != move.owner) {
                        if (type != EMPTY)
                            block++;
                        break;
                    }

                    analyse_move.owner = move.owner;
                    //set只有数字和string类型是判断值 其他对象判断的是引用
                    sets[parseInt(direction / 2)].add(analyse_move.toString());
                    count++;
                }

                // two的索引是0 所以减去2 一共有3种原始状态
                if (direction % 2) {
                    if (block < 2 && count > 1 && count<5) {
                        record[move.owner == BLACK ? 1 : 0][count - 2 + block * 3]++;
                    }
                    block = 0;
                    count = 1;
                }


            }

        }
        return record;

    },

    //player是棋色 内部转为0 1 表示
    score:function (player) {
        var board = this.board;
        var record = this.analysis(board);
        player = player ==BLACK?1:0;
        var opponentPlayer = 1-player;
        //由于下一步是player 所以己方有活四、冲四 也能赢
        //己手活四，必胜
        if (record[player][FOUR])
            return 9000;

        //己手冲四活三
        if (record[player][SFOUR] && record[player][THREE])
        {
            return 8000;
        }

        //己手冲四
        if (record[player][SFOUR])
            return 7000;


        //如果没有上述情况 而
        //对手活四 必败
        if (record[opponentPlayer][FOUR])
        {
            return -9000;
        }

        //对手冲四活三
        if (record[opponentPlayer][SFOUR] && record[opponentPlayer][THREE])
        {
            return -8000;
        }

        //上述情况没有的话
        //活三
        if (record[player][THREE])
        {

            return record[player][THREE] > 1 ? 8000 : 5000;
        }

        if (record[opponentPlayer][THREE])
        {
            return record[opponentPlayer][THREE] > 1 ? -8001 : -5001;
        }
        //上述都没有则算分

        var Turn_Value =0,Opponent_Turn_Value = 0;
        if (record[player][STHREE])
            Turn_Value += record[player][STHREE] * 10;
        if (record[opponentPlayer][STHREE])
            Opponent_Turn_Value += record[opponentPlayer][STHREE] * 10;
        if (record[player][TWO])
            Turn_Value += record[player][TWO] * 4;
        if (record[opponentPlayer][TWO])
            Opponent_Turn_Value += record[opponentPlayer][TWO] * 4;


        return Turn_Value - Opponent_Turn_Value;

},

    IterDeeping: function () {
        var start_time=new Date().getTime();
        var use_time;
        for (var depth = 1;depth<=4;depth=depth * 2)
        {
            var score =  this.alphabeta(depth,-19999,19999,this.type,depth)
            console.log("一次迭代完成《《《")
            console.log("depth:"+depth)
            console.log('bestvalue:'+this.bestMove.information())
            console.log("its value :"+this.bestMove.value)


            // this.bestMove.value = score;

            use_time = new Date().getTime() - start_time;
            console.log('it has used :'+use_time)
            // 在规定的深度内，遇到杀棋，停止思考。
            if( score == 9999)
                break;
            if(use_time/1000>10)
                break;

        }

        console.log('it has used :'+use_time)
        ai.board.savemove(ai.bestMove);
        return use_time;

    },
    //此处player是棋色
    alphabeta:function (depth, alpha,beta,player,MaxDepth) {

        var board = this.board;
        if (depth <= 0)//预判层上的伪叶子结点，计算其启发评价值
        {
            return this.score(player);
        }
        var moveList = this.GenerateMoves()
        if (moveList.length==0){
            this.bestMove = new Move(7,7,BLACK);
            return 9999;
        }
        for(var i = 0;i<moveList.length;i++){
            var move = moveList[i];
            move.owner = player;

            board.savemove(move);
        // 儿子结点为胜负已分状态（真正的叶子结点），表明player方走这一步走法后会获胜
            if(board.isWin()){
                if (depth == MaxDepth )//|| depth == MaxDepth-1)
                {
                    this.bestMove = move;
                }
                board.undolastmove();
                //对杀棋考虑层深
                return 9999+depth;
            }

            move.value = -this.alphabeta(depth - 1, -beta, -alpha,  - player, MaxDepth);

            //验证添加和删去的棋子一样
            board.undolastmove();

            if (DEBUG)
            if (depth == MaxDepth)
            {
                // console.log(" 当前是最顶层！！！！！！！！");
                console.log(" 当前点:"+move.information());
                console.log("value is "+move.value)
                // console.log("alpha is"+alpha);
            }

            if (alpha < moveList[i].value) {

                if (depth == MaxDepth)
                {
                    console.log("DEBUG alpha 更新");
                    console.log("DEBUG best点:"+move.information());
                    console.log("value is "+move.value)
                    console.log("DEBUG last alpha "+alpha);
                }

                alpha = move.value;

                if (depth == MaxDepth)
                {
                    this.bestMove = moveList[i];

                }
            }
            // 剪枝
            if (alpha >= beta) {
                // beta_count++;
                break;
            }
        }


        return alpha;

    },
    GenerateMoves:function () {
        var board = this.board;
        var chesslist = board.chesslist;
        var movelist = [];
        if(chesslist.length == 0)
        {
            return movelist
        }

        var range = chesslist.length<10? 2:1;
        //记录位置是否已加入 不能用等于号直接复制board 因为传的是引用
        var makeSign = new Array(EDGR_NUM);
        for (var x = 0; x < EDGR_NUM; x+=1)
        {
            makeSign[x] = new Array(EDGR_NUM);
            for (var y = 0; y < EDGR_NUM; y+=1)
            {
                makeSign[x][y] = board.chessData[x][y];
            }
        }


        //最近下棋的位置得分高的可能性大
        for ( var i=chesslist.length-1; i>=0; --i ){
            var move =chesslist[i];
            //
            for (var direction = 0; direction < 8; direction++) {
                var analyse_move = move;
                for(var j =0;j<range;j++){
                    analyse_move = analyse_move.getNext(direction);
                    if (!analyse_move)
                    {
                        break;
                    }
                    if(makeSign[analyse_move.x][analyse_move.y])
                        continue;
                    movelist.push(analyse_move);
                    makeSign[analyse_move.x][analyse_move.y]=1;
                }

            }

        }
        return movelist;
    },

}

ai.init(WHITE)
ai.board = board;

this.onmessage = function (e) {
    var cmd = e.data.cmd;

    switch(cmd){
        case "retract":
            ai.board.undolastmove();
            return;
        case "save":
            var move =new Move(e.data.move);
            ai.board.savemove(move);
            break;
        case"init":
            ai.board.init();
            return;
        default:
            break;

    }

    ai.type = ai.board.next_type();
    var time = ai.IterDeeping()
    var data ={'time':time,"move":ai.bestMove}
    this.postMessage(data);
}