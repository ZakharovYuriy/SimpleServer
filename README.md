# SimpleServer
This is the simplest demonstration of using Linux sockets in an application written in C++.

Read in other languages: [English](README.md), [Русский](README.Russian.md)

# Program Description
It is a simple Web server that accepts HTTP requests from clients, usually web browsers, and gives them HTTP responses.<br>
In response to the **GET /** request, the server sends the HTML page and the files necessary for its display, located in the WebPage directory.<br>
For a better understanding of the interaction between the client and the server, incoming and sent information is displayed in stdout.<br>
<br>
The demo page is my brief summary, demonstrating my skills with HTML, CSS and JavaScript.<br>

# Usage
  0. Build the program by following the instructions [below](#build-using-cmake)
  1. Run the server program `./server` in the `Server/` folder
  - The program can be run with or without parameters
  - When starting without parameters, the server will start at the address `127.0.0.1:12345`
  - When starting, you can specify your own address and port to open the socket
  - When starting with the `-h` or `-help` parameter, help about the program startup parameters will be displayed
  2. Open the website in the browser at the address you entered or the standard address `127.0.0.1:12345`

# Build using Cmake
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

# System requirements:
  0. C++17(STL)<br>
  1. GCC (MinG w64) 11.2.0 <br>
  2. OS Linux <br>
  
# Technology stack:
  0. Berkeley sockets<br>
  1. TCP/IP
  2. HTTP запросы
  3. HTML, CSS, JavaScript 

	
	
