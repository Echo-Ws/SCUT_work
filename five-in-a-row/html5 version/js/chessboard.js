/**
 * Created by Administrator on 2016/10/27.
 */

class Move{
    constructor(){
        switch (arguments.length){
            case 1:
                this.x = arguments[0].x;
                this.y = arguments[0].y;
                this.owner =arguments[0].owner;
                break;
            case 2:
                this.x = arguments[0];
                this.y = arguments[1];
                break;
            case 3:
                this.x = arguments[0];
                this.y = arguments[1];
                this.owner = arguments[2];
        }



    }

    getNext(direction){
        switch(direction){
            case LH :
                if(this.x-1<0)
                    return false;
                return new Move(this.x-1,this.y,this.owner);
            case RH:
                if(this.x+1>=EDGR_NUM)
                    return false
                return new Move(this.x+1,this.y,this.owner);
            case TOP:
                if(this.y-1<0)
                    return false;
                return new Move(this.x,this.y-1,this.owner);
            case DOWN:
                if(this.y+1>=EDGR_NUM)
                    return false;
                return new Move(this.x,this.y+1,this.owner);
            case LLS:
                if(this.x-1<0 || this.y-1<0)
                    return false;
                return new Move(this.x-1,this.y-1,this.owner);
            case RLS:
                if(this.x+1>=EDGR_NUM || this.y+1>=EDGR_NUM )
                    return false;
                return new Move(this.x+1,this.y+1,this.owner);
            case LRS:
                if(this.x-1<0 || this.y+1>=EDGR_NUM)
                    return false;
                return new Move(this.x-1,this.y+1,this.owner);
            case RRS:
                if(this.x+1>=EDGR_NUM || this.y-1<0)
                    return false;
                return new Move(this.x+1,this.y-1,this.owner);
        }


    }

    toString(){
        return this.x.toString()+this.y.toString()
    }
    information(){
        return "x: "+this.x+"  y:"+this.y+'  owner is '+this.owner;
    }
}

var board = {

    init : function(){
        //为悔棋服务
        this.chesslist=[];

        this.chessData = new Array(EDGR_NUM);
        //这个为棋盘的二维数组用来保存棋盘信息，初始化0为没有走过的，-1为白棋走的，1为黑棋走的
        for (var x = 0; x < EDGR_NUM; x+=1)
        {
            this.chessData[x] = new Array(EDGR_NUM);
            for (var y = 0; y < EDGR_NUM; y+=1)
            {
                this.chessData[x][y] = EMPTY;
            }
        }

    },
    //下一个需要棋子的类型
    next_type:function () {
        var len = this.chesslist.length;
        if(len)
            return -this.chesslist[len-1].owner;
        return BLACK;
    },
    //检查当前位子是否有子
    isEmpty : function(move) {
    return !  this.chessData[move.x][move.y];
    },

    //绘制棋盘的线
    drawboard :function(){
        cxt.clearRect(0, 0, 700, 700);
        //绘制棋盘的线
        for (var i = MARGIN; i < MARGIN+INTERVAL*(EDGR_NUM); i += INTERVAL) {
            cxt.beginPath();
            cxt.moveTo(MARGIN, i);
            cxt.lineTo(MARGIN+INTERVAL*(EDGR_NUM-1), i);
            cxt.closePath();
            cxt.stroke();
            cxt.beginPath();
            cxt.moveTo(i, MARGIN);
            cxt.lineTo(i, MARGIN+INTERVAL*(EDGR_NUM-1));
            cxt.closePath();
            cxt.stroke();
        }
        //绘制棋子
        for (var x of this.chesslist){
            this.drawchess(x);
        }
    },

    drawchess : function(move){
        if(move.owner == BLACK)
            var img = img_b;
        else
            var img = img_w;
        console.log("画布坐标"+(move.x*INTERVAL+MARGIN)+ "  "+(move.y*INTERVAL+MARGIN))
        cxt.drawImage(img,move.x*INTERVAL+MARGIN/32,move.y*INTERVAL+MARGIN/32);

    },

    savemove: function (move) {
        this.chessData[move.x][move.y] = move.owner;
        this.chesslist.push(move);
    },

    savechess : function(move) {
        if(move.x>=EDGR_NUM||move.y>=EDGR_NUM ||move.x<0||move.y<0){
            set_status("点击位置不在棋盘范围")
            return false;
        }
        if (! this.isEmpty(move)) {//判断该位置是否被下过了
            alert("该位置已经有子，你不能在这个位置下棋");
            return false;
        }
        this.savemove(move);
        this.drawchess(move);
        if (this.isWin()){
            if(move.owner ==BLACK)
            {
                set_status("黑棋获胜,请点击开始重新开始");
                return 2
            }

            else {
                set_status('白棋获胜，请点击开始重新开始');
                return 3
            }
        }
        return true;
    },

    isWin : function(){
        var move = this.chesslist[this.chesslist.length-1]
        var player = move.owner;
        var count = 1;
        //检查横
        for(var i=1;i<5;i++){
            if (move.x-i<0 || this.chessData[move.x-i][move.y] != player)
                break;
            count++;
        }
        for(var i=1;i<5;i++){
            if (move.x+i>=EDGR_NUM||this.chessData[move.x+i][move.y]!= player)
                break;
            count++;
        }
        if (count>=5) return 1;

        //检查竖
        count = 1;
        for(var i=1;i<5;i++){
            if (move.y-i<0 || this.chessData[move.x][move.y-i] != player)
                break;
            count++;
        }
        for(var i=1;i<5;i++){
            if (move.y+i>=EDGR_NUM||this.chessData[move.x][move.y+i] != player)
                break;
            count++;
        }
        if (count>=5) return 1;

        //检查左斜
        count = 1;
        for(var i=1;i<5;i++){
            if (move.x+i>=EDGR_NUM||move.y-i<0 ||this.chessData[move.x+i][move.y-i] != player)
            break;
            count++;
        }
        for(var i=1;i<5;i++){
            if (move.x-i<0 || move.y+i>=EDGR_NUM||this.chessData[move.x-i][move.y+i] != player)
                break;
            count++;
        }
        if (count>=5) return 1;

        //检查右斜
        count = 1;
        for(var i=1;i<5;i++){
            if (move.x+i>=EDGR_NUM||move.y+i>=EDGR_NUM||this.chessData[move.x+i][move.y+i] != player)
                break;
            count++;
        }
        for(var i=1;i<5;i++){
            if (move.x-i<0||move.y-i<0 ||this.chessData[move.x-i][move.y-i]!= player)
                break;
            count++;
        }
        if (count>=5) return 1;
        return 0;

    },

    undolastmove:function () {
        var move = this.chesslist.pop();
        this.chessData[move.x][move.y] = EMPTY;
        return move;
    },

    retract:function(){
        if(! this.chesslist.length){
            set_status("当前棋盘没有棋子");
            return 0;
        }
        var move = this.undolastmove();
    },


}

board.init();



