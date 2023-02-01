let button_send = document.getElementById('button_send');

button_send.onmouseover=over;
button_send.onmouseout=out;
var x_col=0;
var y_col=46;
var z_col=100;


function over (event) {
	button_send.style.backgroundImage = 'linear-gradient(270deg, #4158D0 0%, #C850C0 46%, #FFCC70 100%)';
}

function out (event) {
	//console.log("out");
	button_send.style.backgroundImage = 'linear-gradient(90deg, #4158D0 0%, #C850C0 46%, #FFCC70 100%)';
}

