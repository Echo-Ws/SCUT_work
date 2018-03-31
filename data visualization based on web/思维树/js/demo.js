var jsonParam={"mall":"京东"};
var url="http://119.29.158.92/shunfeng/getAllTypeDetail";
//svg size
var width = document.body.clientWidth;
var height = 1000;

//textSize
var max = 18;
var min = 8;

//边界空白
var padding = {left: 50, right: 50, top: 20, bottom: 20};

var svg = d3.select("body")
    .append("svg")
    .attr("width", width + padding.left + padding.right)
    .attr("height", height + padding.top + padding.bottom)
    .append("g")
    .attr("transform", "translate(" + padding.left + "," + padding.top + ")");

//树状图布局size() 设定尺寸，即转换后的各节点的坐标在哪一个范围内。
var tree = d3.layout.tree()
    .size([width, height]);

//对角线生成器
var diagonal = d3.svg.diagonal()
    .projection(function (d) {
        return [d.x, d.y];
    });

var nid = 0;

// $.ajax({
//     type: 'GET',
//     url: "http://116.56.143.18:8080/relationWeb/relation/getTopNDegreeRelations?id=30&degree=4",
//     // data: jsonParam,
//     success: function (root) {
//         //给第一个节点添加初始坐标x0和x1
//         root.x0 = height / 2;
//         root.y0 = 0;

//         //初始化使每个节点一开始收起 且绑定一个id使得同名项可以显示
//         init(root);
//         //重绘函数
//         redraw(root,root);
//         },

//     dataType: "json"
// });




d3.json("JD2.json",function (error,root) {

 //给第一个节点添加初始坐标x0和x1
 root.x0 = height / 2;
 root.y0 = 0;

 //初始化使每个节点一开始收起 且绑定一个id使得同名项可以显示
 init(root);
 //重绘函数
 redraw(root,root);



    }
 );


function redraw(source,root) {

    /*
     （1） 计算节点和连线的位置
     */

    //应用布局，计算节点和连线
    var nodes = tree.nodes(root);
    var links = tree.links(nodes);


    //重新计算节点的y坐标 当数据更新 保证树状图的结构不要发生太大的变化
    nodes.forEach(function (d) {
        d.y = d.depth * 180 + max;
    });
    /*
     更新点 线
     */

    //获取节点的update部分
    var nodeUpdate = svg.selectAll(".node")
        .data(nodes, function (d) {
            return d.id;
        });//function设置在该元素作为主键的数据


    //获取节点的enter部分
    var nodeEnter = nodeUpdate.enter();

    //获取节点的exit部分
    var nodeExit = nodeUpdate.exit();


    //获取连线的update部分
    var linkUpdate = svg.selectAll(".link")
        .data(links, function (d) {
            return d.target.id;
        });

    //获取连线的enter部分
    var linkEnter = linkUpdate.enter();

    //获取连线的exit部分
    var linkExit = linkUpdate.exit();


    /*
     （2） 节点的处理
     */


    // console.log(nodeExit)
    //1. 节点的 Enter 部分的处理办法
//    每一个新添加的节点都将缓慢地过渡到自己本身的位置，如此更具有友好性。因此，新节点的初始位置都设定在source节点处
    var enterNodes = nodeEnter.append("g")
        .attr("class", "node")
        .attr("transform", function (d) {
            return "translate(" + source.x0 + "," + source.y0 + ")";
        })
        .on("click", function (d) {
            if (d.children || d._children) {

                // 一次只能展开一个项
                if (d.parent) {
                    d.parent.children.forEach(function (sub) {
                        if (sub.children && sub != d)toggle(sub);
                    })
                }
                toggle(d);
                //实现字体大小变化
                if (d.children) {
                    if (d.parent) {
                        d.parent.children.forEach(function (d) {
                            d.textSize = min;
                        })
                    } else d.textSize = min;

                } else if (d.parent) {
                    d.parent.children.forEach(function (d) {
                        d.textSize = max;
                    })
                } else d.textSize = max;

                redraw(d,root);
            }else{
                console.log(d);

            }
        })
        .on("mouseover", function (d) {
            linkUpdate.attr("color", function (link) {

                if (d.id == link.target.id) {
                    d3.select(this).style("stroke", "red");
                    // console.log(link.target.name + "turn red" + d.name);
                }
                // console.log(link);


            })


        })
        .on("mouseout", function (d) {

            linkUpdate.attr("color", function (link) {
                if (d.id == link.target.id && !d.children)
                    d3.select(this).style("stroke", "#ccc");
                // console.log(link);

            });

        });

    enterNodes.append("circle")
        .attr("r", 0)
        .style("fill", function (d) {
            return d._children ? "lightsteelblue" : "white";
        });

    var Text=enterNodes.append("text")
        .attr("y", 14)
        // .attr("x", "10")
        .attr("text-anchor","middle")

        .style("fill-opacity", 0)
        .on("mouseover", function () {
            d3.select(this).attr("fill", "red");

        })
        .on("mouseout", function () {
            d3.select(this)
                .attr("fill", "black");

        });

    Text.selectAll("tspan")
        .data(function (d) {
            return d.name;
        })
        .enter()
        .append("tspan")
        .attr("x",0 )
        .attr("dy","1em")
        .text(function(d){

            return d;
        });


    //2. 节点的 Update 部分的处理办法
    var updateNodes = nodeUpdate.transition()
        .duration(500)
        .attr("transform", function (d) {
            return "translate(" + d.x + "," + d.y + ")";
        });

    updateNodes.select("circle")
        .attr("r", 8)
        .style("fill", function (d) {
            return d._children ? "lightsteelblue" : "white";
        });

    updateNodes.select("text")
        .style("fill-opacity", 1)
        .style("font-size", function (d) {
            return d.textSize + "px";
        });



    //3. 节点的 Exit 部分的处理办法
    var exitNodes = nodeExit.transition()
        .duration(500)
        .attr("transform", function (d) {
            return "translate(" + source.x + "," + source.y + ")";
        })
        .remove();

    exitNodes.select("circle")
        .attr("r", 0);

    exitNodes.select("text")
        .style("fill-opacity", 0);

    /*
     （3） 连线的处理
     */


    //1. 连线的 Enter 部分的处理办法
    linkEnter.insert("path", ".node")
        .attr("class", "link")
        .attr("d", function (d) {
            var o = {x: source.x0, y: source.y0};
            // console.log(d.target.name+" "+source.name);
            return diagonal({source: o, target: o});
        })
        .transition()
        .duration(500)
        .attr("d", diagonal);

    //2. 连线的 Update 部分的处理办法
    linkUpdate.transition()
        .duration(500)
        .attr("d", diagonal);


    //3. 连线的 Exit 部分的处理办法
    linkExit.transition()
        .duration(500)
        .attr("d", function (d) {
            var o = {x: source.x, y: source.y};
            return diagonal({source: o, target: o});
        })
        .remove();

    linkUpdate.style("stroke", function (link) {
        if (link.target.children){
            // console.log(link.target.name + "turn red" + d.name);
            return "red";
        }
        if (!link.target.children)return "#ccc";
    });
    /*
     （4） 将当前的节点坐标保存在变量x0、y0里，以备更新时使用
     */
    nodes.forEach(function (d) {
        d.x0 = d.x;
        d.y0 = d.y;
    });

}

//切换开关，d 为被点击的节点
function toggle(d) {
    if (d.children) { //如果有子节点
        d._children = d.children; //将该子节点保存到 _children
        d.children = null;  //将子节点设置为null

    } else {  //如果没有子节点
        d.children = d._children; //从 _children 取回原来的子节点
        d._children = null; //将 _children 设置为 null
    }
}

function init(source) {

    source.id = nid;
    nid++;
    if (source.children)
        source.children.forEach(function (d) {
            init(d);
        });
    else return;
    source._children = source.children; //将该子节点保存到 _children
    source.children = null;  //将子节点设置为null
}