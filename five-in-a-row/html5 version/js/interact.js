/**
 * Created by Administrator on 2016/11/9.
 */

board.drawboard();
var AI = new Worker('./js/evaluate.js');
var ai_think=0;

var win = 0;

AI.onmessage = function (e) {
    var time = e.data.time;
    var  bestmove = new Move(e.data.move);
    var s = board.savechess(bestmove);

    if(s==2){
        win =2;
        alert("黑棋获胜,请点击开始重新开始. Black Win!")

    }else if(s==3){
        win = 3;
        alert("白棋获胜,请点击开始重新开始. White Win！")

    }

    set_status("ai 下在"+bestmove.information()+' ised time： '+time+'ms');
    ai_think=0;
};

function command(cmd, ...rest) {
    return {"cmd":cmd,"move":rest[0]};
}

function set_status(s) {
    $("#status").html(s);
}

$("#huiqi").click(function () {
    if (win !=0){
        set_status('Gameover. Please press start')
        return
    }
    if(ai_think){
        set_status("AI is thinking. Please be patient.")
        return
    }
    board.retract();
    board.retract();
    //重绘棋盘
    board.drawboard();
    AI.postMessage(command("retract"))
});

$("#start").click(function () {
    var player_type = $("#type").val();  //获取Select选择的Value
    var type = player_type==-1?"white chess":"black chess"
    set_status("Game begin, you are using:"+type)
    win =0;
    cxt.clearRect(0, 0, 700, 700);
    board.init();
    board.drawboard();
    AI.postMessage(command("init"));

    if (player_type == -1){
        AI.postMessage(command(0));
    }
});
   //  cons
canvas.onclick=function(e){//给canvas添加点击事件
    if (win !=0){
        set_status('Gameover. Please press start')
        return
    }
    if(ai_think){
        set_status("AI is thinking. Please be patient！！")
        return
    }
    e=e||event;//获取事件对象
    //获取事件在canvas中发生的位置
    var bbox = canvas.getBoundingClientRect();
    var x=Math.round((e.clientX-bbox.left-MARGIN)/INTERVAL)*INTERVAL+MARGIN;
    var y=Math.round((e.clientY-bbox.top-MARGIN)/INTERVAL)*INTERVAL+MARGIN;



    var move = new Move( (x-MARGIN)/INTERVAL,(y-MARGIN)/INTERVAL,board.next_type());
    console.log("棋盘坐标"+move.x+"　"+move.y);

    // var a = move.getNext(LH)

    console.log("原始坐标"+x+"　"+y);
    var s = board.savechess(move);

    if(s) {
        if(s==2){
            win =2;

            alert("黑棋获胜,请点击开始重新开始. Black Win!")
            return
        }else if(s==3){
            win =3;

            alert("白棋获胜,请点击开始重新开始. White Win！")
            return
        }


        AI.postMessage( command('save',move));

        ai_think=1;
        set_status("AI is thinking. Please be patient.")
    }



    // console.log("原生坐标"+(e.clientX-bbox.left-MARGIN)/40+"　"+(e.clientY-canvas.offsetTop- bbox.top-MARGIN)/40);
    // console.log("is empty?:"+isEmpty(human_move));


}