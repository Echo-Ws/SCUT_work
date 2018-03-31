var divid='#chart'
var w = 190,
	h = 190;

var colorscale = d3.scale.category10();

var mycfg = {
	w: w,
	h: h,
	maxValue: 10,
	levels: 5,
	
	radius: 2,//控制雷达图上点的大小
	linewidth:"1px",//雷达上的线宽
	factor: 1,//雷达图大小
	factorLegend: 0.85,//标签与雷达图的距离
	radians: 2 * Math.PI,
	opacityArea: 0.2,//填充色的透明度
	ToRight: 1,//刻度距离雷达图的距离
	TranslateX: 80,//雷达图在网页上的位置
	TranslateY: 80,
	ExtraWidthX: 200,
	ExtraWidthY: 150,//增加svg大小
}



var d=[];	
var LegendOptions =[];
	d3.csv("data.csv",function(data){	//'data.csv'	
		
			//console.log(data);
			//存取每一个公司的产品值
			
			data.forEach(function(x,y){
			var count=0;
			var item=[];
			for(var key in x)
				{
				count+=1;	
				if (count==1)
				{LegendOptions.push(x[key]);}
				else{
				// var tmp={axis:key,value:x[key]}
				var tmp={axis:key,value:Math.random()*10}
				item.push(tmp);
					}
				
				}
		d.push(item);				
		
		});
	RadarChart.draw(divid, d, mycfg);
	var svg = d3.select(divid)
	.selectAll('svg')
	.append('svg:g')
	.attr("width", w+100)
	.attr("height", h);

	//Create the title for the legend
	var text = svg.append("text")
		.attr("class", "title")
		.attr('transform', 'translate(90,0)') 
		.attr("x", w - 70)
		.attr("y", 10)
		.attr("font-size", "12px")
		.attr("fill", "#ddd")
		.text("雷达图");
			
	//Initiate Legend	图例
	var legend = svg.append("g")
		.attr("class", "legend")
		.attr("height", 100)
		.attr("width", 200)
		.attr('transform', 'translate(90,20)') 
		;
		//Create colour squares
		legend.selectAll('rect')
		  .data(LegendOptions)
		  .enter()
		  .append("rect")
		  .attr("x", w - 65)
		  .attr("y", function(d, i){ return i * 18;})
		  .attr("width", 10)
		  .attr("height", 10)
		  .style("fill", function(d, i){ return colorscale(i);})
		  ;
		//Create text next to squares
		legend.selectAll('text')
		  .data(LegendOptions)
		  .enter()
		  .append("text")
		  .attr("x", w - 52)
		  .attr("y", function(d, i){ return i * 18 +9;})
		  .attr("font-size", "11px")
		  .attr("fill", "#aaa")
		  .text(function(d) { return d; })
		  .on('mouseover', function (d,i){
				z = "polygon.radar-chart-serie"+i;
				// console.log(svg.selectAll('.'+z));
			d3.select("svg").selectAll("polygon")
			.transition()
			.duration(500)
			.style("fill-opacity", 0.1)
			.style("stroke-width", 0); 
			 
			d3.select("svg").select(z)
			.transition()
			.duration(500)
			.style("fill-opacity", 0.8)
			.style("stroke-width", mycfg.linewidth);
		
			
			
			
	// console.log(z);
	// console.log(svg.selectAll(z));
	// alert(svg.selectAll(z).length);
	})
	.on('mouseout', function(){
			d3.select("svg").selectAll("polygon")
			.transition()
			.duration(500)
			.style("stroke-width", mycfg.linewidth)
			.style("fill-opacity", mycfg.opacityArea);
		});
	}
)


	
		
//Options for the Radar chart, other than default

//Call function to draw the Radar chart
//Will expect that data is in %'s


////////////////////////////////////////////
/////////// Initiate legend ////////////////
////////////////////////////////////////////

