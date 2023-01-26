document.onmousemove = move;
window.onresize = resize;

var pageWidth = document.documentElement.clientWidth
var pageHeight = document.documentElement.clientHeight

//обработка изменения размера окна
function resize(event) {
pageWidth = document.documentElement.clientWidth
pageHeight = document.documentElement.clientHeight
//console.log(pageWidth+':'+pageHeight)
}
//вращение фона при перемещении мыши
function move (event) {
	let x = event.clientX - pageWidth / 2;
	let y = pageHeight / 2 - event.clientY;
	var deg = 0 - (180 / Math.PI ) * Math.acos(x/Math.sqrt(x*x+y*y));
	//console.log(x + ':' + y + ' deg:' + deg);
	
	if(y<0){
		deg = -deg;
	}
	
	let all_doc = document.getElementById('all_doc');
	all_doc.style.backgroundImage = 'linear-gradient(' +deg +'deg, #4158D0 0%, #C850C0 46%, #FFCC70 100%)';
}
