let button_send = document.getElementById('button_send');

button_send.onmouseover = over;
button_send.onmouseout = out;
button_send.onclick = click;
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

var xhr1 = new XMLHttpRequest();
var xhr2 = new XMLHttpRequest();
function click (event) {
	console.log("click");
	
		xhr1.open("POST", 'Mython', true);
		xhr1.setRequestHeader('Content-Type', 'application/x-www-form-urlencoded');
		var body = document.getElementById('code_block').value;
		xhr1.onloadend = state;
		xhr1.send(body);
		
		console.log(body);		
}

function state (event) {
	console.log("ready");
	xhr2.open("GET", 'text.txt', true);
	xhr2.setRequestHeader('Content-Type', 'application/x-www-form-urlencoded');
	xhr2.send();
}

xhr2.onprogress = function(event) { // запускается периодически
  // event.loaded - количество загруженных байт
  // event.lengthComputable = равно true, если сервер присылает заголовок Content-Length
  // event.total - количество байт всего (только если lengthComputable равно true)
  console.log(`Загружено ${event.loaded} из ${event.total}`);
  if(event.loaded==event.total){
	  //console.log(xhr.responseText);
	  document.getElementById('answer_block').value += xhr2.responseText;
	}
};
