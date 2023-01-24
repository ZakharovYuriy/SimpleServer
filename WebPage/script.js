const para = document.querySelector('p');

      para.addEventListener('click', updateName);

      function updateName() {
        const name = prompt('Enter a new name');
        para.textContent = `Player 1: ${name}`;
        
        // 1. Создаём новый объект XMLHttpRequest
	var xhr = new XMLHttpRequest();

	// 2. Конфигурируем его: GET-запрос на URL 'phones.json'
	xhr.open('LOL', 'phones.json', false);

	// 3. Отсылаем запрос
	xhr.send();
      }
