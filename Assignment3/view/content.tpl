<h1 style="text-align: center;">Coffee Maker</h1>
<div id="placeholder" style="width:800px;height:300px;"></div>

    <p>Coffee consumption data. Explain the application.</p>

<script language="javascript" type="text/javascript" src="static/js/flot/jquery.js"></script>
<script language="javascript" type="text/javascript" src="static/js/flot/jquery.flot.js"></script>
<script language="javascript" type="text/javascript">

// Example from the flot homepage
// http://www.flotcharts.org/
$(function() {

	var container = $("#placeholder");

	// Determine how many data points to keep based on the placeholder's initial size;
	// this gives us a nice high-res plot while avoiding more than one point per pixel.

	var maximum = container.outerWidth() / 2 || 300;

	//

	var data = [];

    function getRandomData() {

        if (data.length) {
            data = data.slice(1);
        }

        while (data.length < maximum) {
            var previous = data.length ? data[data.length - 1] : 50;
            var y = previous + Math.random() * 10 - 5;
            data.push(y < 0 ? 0 : y > 100 ? 100 : y);
        }

        // zip the generated y values with the x values

        var res = [];
        for (var i = 0; i < data.length; ++i) {
            res.push([i, data[i]])
        }

        
        return res;
    }

	//

	series = [{
		data: [], // getRandomData(),
		lines: {
			fill: true
		}
	}];

	//

	var plot = $.plot(container, series, {
		grid: {
			borderWidth: 1,
            color : "#a1804c",
			minBorderMargin: 20,
			labelMargin: 10,
			backgroundColor: {
				colors: ["#fff", "#745626"]
			},
			hoverable: false,
			mouseActiveRadius: 50,
			margin: {
				top: 8,
				bottom: 20,
				left: 20
			},
			markings: function(axes) {
				var markings = [];
				var xaxis = axes.xaxis;
				for (var x = Math.floor(xaxis.min); x < xaxis.max; x += xaxis.tickSize * 2) {
					markings.push({ xaxis: { from: x, to: x + xaxis.tickSize }, color: "rgba(232, 232, 255, 0.2)" });
				}
				return markings;
			}
		},
		xaxis: {
			min: 0,
			max: 1
		},
		yaxis: {
			min: 0,
			max: 1023
		},
		legend: {
			show: true
		}
	});

	// Create the demo X and Y axis labels

	//var xaxisLabel = $("<div class='axisLabel xaxisLabel'></div>")
	//	.text("")
	//	.appendTo(container);

	var yaxisLabel = $("<div class='axisLabel yaxisLabel'></div>")
		.text("Response Time (ms)")
		.appendTo(container);

	// Since CSS transforms use the top-left corner of the label as the transform origin,
	// we need to center the y-axis label by shifting it down by half its width.
	// Subtract 20 to factor the chart's bottom margin into the centering.

	yaxisLabel.css("margin-top", yaxisLabel.width() / 2 - 20);

	// Update the random dataset at 25FPS for a smoothly-animating chart

	setInterval(function updateRandom() {
		series[0].data = getRandomData();
		$.getJSON('json_last1h', function(data) {
			var plotData = [{
                    data: [],
                    lines: {
                        fill: true
                    }
                }],
                i,
                len = data.length,
                hour,
                min,
                sec,
                x_value,
                y_value,
                values = [];
                
            hour = parseInt(data[0][3]);
            min = parseInt(data[0][4]);
            sec = parseInt(data[0][5]);
            x_init = hour * 3600 + min * 60 + sec;
            
            hour = parseInt(data[len-1][3]);
 			min = parseInt(data[len-1][4]);
            sec = parseInt(data[len-1][5]);
            x_last = hour * 3600 + min * 60 + sec - x_init;
            
            for(i = 0; i < len; i += 1) {
                hour = parseInt(data[i][3]);
                min = parseInt(data[i][4]);
                sec = parseInt(data[i][5]);
                x_value = parseFloat(hour * 3600 + min * 60 + sec - x_init) / x_last;
                y_value = data[i][6];
                console.log(x_value, y_value);
                plotData[0].data.push([x_value, y_value])
            }
        	
        	console.log(plotData);

        	// plot.setData(series);
            plot.setData(plotData);
			plot.draw();
        });
		
	}, 1000);

});
</script>
%rebase view/layout title='Last 1 hour'