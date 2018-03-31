/**
 * Created by Administrator on 2016/5/13.
 */
var jsonParam=
{   "mall":0,
    "topId":"TR_MY",
    "typeId1":"毛衣",
    // "typeId2":"国产水果",
    "page":1,
    "length":20
};
var url="http://119.29.158.92/shunfeng/getAllGoods";

$.ajax({
    type: "GET",
    url: url,
    data: jsonParam,
    success:function(data){
        var rectHeight = 33;	//每个矩形所占的像素高度(包括空白)
        var width="100%";
        var height;

        data = data.goods;
        data.forEach(function (d,i) {
            // d.turnoverIndex = (20-i)*100;
            d.y =i * rectHeight;
        });
        console.log(data);

        var rectMax = d3.max(data,function (d) {
            return d.turnoverIndex;
        });
        var textMax = d3.max(data,function (d) {
            return d.title.length;
        })


        var rankX=30;//中间宽度
        var rectX=textMax*10;//矩阵起点的X 整体的x位置由他控制 10为字体name大小
        var gap=10;//上下两项的间隔




        var svg = d3.select("body")
                    .append("svg")
                    .attr("width",width);


    console.log("最长的字"+textMax);

        //线性比例尺
        var linear = d3.scale.linear()
            .domain([0, rectMax])
            .range([0, 250]);


        svg.selectAll("rect")
            .data(data)
            .enter()
            .append("rect")
            .attr("x",rectX)
            .attr("y",function(d,i){
                // console.log(d)
                return d.y;
            })
            .attr("height",rectHeight-gap)
            .attr("width",0)
            .attr("fill","white")
            .transition()
            .delay(function(d,i){
                return i * 200;
            })
            .duration(2000)
            .ease("elastic")
            .attr("width",function(d) {
                return linear(d.turnoverIndex);
            })

            .attr("fill","steelblue");

        var TextG=svg.selectAll("g")
            .data(data)
            .enter()
            .append("g")
            .attr("class","textGroup")
            .attr("transform",function (d) {
                return "translate(" + (rectX-rankX)+ "," + (d.y+gap+5) + ")";
            });

        TextG.append("a")
            .attr("xlink:href",function (d) {
                return d.url
            })
            .append("text")
            .attr("class","name")
            .text(function (d) {
                return d.title;
            });

        TextG.append("text")
            .attr("class","rank")
            .text(function (d) {
                return d.rank;
            })
            .attr("x",rankX/2);

        TextG.append("text")
            .attr("class","sales")
            .text(function (d) {
                return d.turnoverIndex;
            })
            .attr("x",0)
            .transition()
            .delay(function(d,i){
                return i * 200;
            })
            .duration(2000)
            .ease("elastic")
            .attr("x",function (d) {
                return linear(d.turnoverIndex)+rankX-1;
            });

        height=Number($("svg rect:last").attr("y"))+rectHeight;

        var buttom = svg.append("g")
                        .attr("transform","translate("+ (rectX-rankX/2)+ "," + (height+gap) + ")")
                        .attr("class","bottomTitle");

        buttom.append("text")
                .text("销量")
                .attr("y",gap)
                .attr("text-anchor","middle")
        buttom.append("text")
                .attr("text-anchor","middle")
                .text("排行")
                .attr("y",2.5*gap+2);

        buttom.append("text")
            .text("销量")
            .attr("class","sub")
            .attr("x",rankX/2)
            .attr("y",-6);

        buttom.append("text")
            .text("产品名")
            .attr("class","sub")
            .attr("x",-rankX/2)
            .attr("y",-6)
            .attr("text-anchor","end");






        console.log(height);


      /* 废弃的坐标轴方案
            var axis = d3.svg.axis()
            .scale(linear)		//指定比例尺
            .orient("bottom")	//指定刻度的方向
            .ticks(5);			//指定刻度的数量

        var bottom=svg.append("g")
            .attr("class","axis")
            .attr("transform","translate("+rectX+","+height+")")
            .call(axis);*/
   


        //确定了最终高度后
        height+=40;
        svg .attr("height",height);
    },
    dataType: "json"
})