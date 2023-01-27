# SimpleServer
Это простейшая демонстрация использования сокетов ОС Linux в приложении написаном на С++.

Читать на других языках: [English](README.md), [Русский](README.Russian.md)

# Описание программы
Это простой Веб-сервер, принимающий HTTP-запросы от клиентов, обычно веб-браузеров, и выдающий им HTTP-ответы.<br>
Данный сервер может принимать запросы из локальной сети по адресу 127.0.0.1:12345<br>
В ответ на запрос **GET /** сервер посылает HTML сраницу и необходимые для ее отображения файлы, находящиеся в каталоге WebPage.<br>
Для лучшего понимания взаимодействия клиента и сервера - в stdout выводится поступающая и отправляемая информация.<br>

# Сборка при помощи Cmake
To build this project on linux you need:<br>
1)If you don't have Cmake installed, install Cmake<br>
2)Run the command for Debug and Release conf:<br>

```
mkdir CmakeFiles
cmake -E chdir CmakeFiles/ cmake -G "Unix Makefiles" ../ -DCMAKE_BUILD_TYPE:STRING=Release
```
3)Build command:<br>

```
cmake --build CmakeFiles/.
```
4)To **Run** program- go to (cd Server/) folder and run:<br>

```
./serrver
```

# Системые требования:
  0. C++17(STL)<br>
  1. GCC (MinG w64) 11.2.0 <br>
  2. OS Linux <br>
  
# Планы по доработке:


# Стек технологий:
  0. Сокеты Беркли<br>

# Использование
## Перед тем как начать:
  0. Установка и настройкка всех требуемых компонентов в среде разработки длля запуска приложения
  
# Описание возможностей:


