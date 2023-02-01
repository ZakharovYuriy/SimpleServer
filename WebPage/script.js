document.onmousemove = move;
window.onresize = resize;

var pageWidth = document.documentElement.clientWidth
var pageHeight = document.documentElement.clientHeight

var scrolling = false;

//обработка изменения размера окна
function resize(event) {
pageWidth = document.documentElement.clientWidth
pageHeight = document.documentElement.clientHeight
//console.log(pageWidth+':'+pageHeight)
}
//вращение фона при перемещении мыши
function move (event) {
	//rotation
	/*let x = event.clientX - pageWidth / 2;
	let y = pageHeight / 2 - event.clientY;
	var deg = 0 - (180 / Math.PI ) * Math.acos(x/Math.sqrt(x*x+y*y));
	//console.log(x + ':' + y + ' deg:' + deg);
	
	if(y<0){
		deg = -deg;
	}*/
	
	//only left-right moution
	let x = event.clientX - pageWidth / 2;
	var deg = (180/pageWidth * x)/3;
	
	let all_doc = document.getElementById('all_doc');
	all_doc.style.backgroundImage = 'linear-gradient(' +deg +'deg, #4158D0 0%, #C850C0 46%, #FFCC70 100%)';
}

var scroll_zone = 0;

window.addEventListener('scroll', function() {
	var position = window.scrollY;
	
	let temp_pos;
	if(position < 400){
		temp_pos = 0;
	}else if(position >= 400 && position <= 1000){
		temp_pos = 1;
	}else{
		temp_pos = 2;
	}
	
	if(scroll_zone != temp_pos){
		scroll_zone=temp_pos;
		console.log(position + "scroll_zone :" +scroll_zone);
		let elem = document.getElementById('about_me');
		console.log(elem.getBoundingClientRect());
		/*window.scrollBy({
			  top: 200,
			  left: 0,
			  behavior: 'smooth'
			});*/
	}
	
	
	/*if(position > 400 && position<1000){
		window.scrollBy({
			  top: 1,
			  left: 0,
			  behavior: 'smooth'
			});
	}*/

});

let call_mython = document.getElementById('Call_Mython');

if (call_mython){
call_mython.onclick = CallMython;}

function CallMython (event) {
	console.log("CallMython");
	window.open("./pages/mython.html");
}



