

This is the page content: {{content}}
<div id="placeholder" style="width:600px;height:300px;"></div>

    <p>Simple example. You don't need to specify much to get an
       attractive look. Put in a placeholder, make sure you set its
       dimensions (otherwise the plot library will barf) and call the
       plot function with the data. The axes are automatically
       scaled.</p>

<script language="javascript" type="text/javascript" src="static/js/flot/jquery.js"></script>
<script language="javascript" type="text/javascript" src="static/js/flot/jquery.flot.js"></script>
<script language="javascript" type="text/javascript">
$(function () {
    var d1 = [];
    for (var i = 0; i < 14; i += 0.5)
        d1.push([i, Math.sin(i)]);

    var d2 = [[0, 3], [4, 8], [8, 5], [9, 13]];

    // a null signifies separate line segments
    var d3 = [[0, 12], [7, 12], null, [7, 2.5], [12, 2.5]];
    
    var p = $.plot($("#placeholder"), [ d1, d2, d3 ]);
    
    // Label on series
    // http://stackoverflow.com/questions/1174298/flot-data-labels
    $.each(p.getData()[0].data, function(i, el){
        var o = p.pointOffset({x: el[0], y: el[1]});
        $('<div class="data-point-label">' + el[1] + '</div>').css( {
            position: 'absolute',
            left: o.left + 4,
            top: o.top - 43,
            display: 'none'
        }).appendTo(p.getPlaceholder()).fadeIn('slow');
    });
});
</script>
%rebase layout title='Content Title'