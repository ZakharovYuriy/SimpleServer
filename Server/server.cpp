#include <sys/socket.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <unistd.h>
#include <arpa/inet.h>

#include <iostream>

//обработка прерываний
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

#include <string>
#include "request_processing.h"
#include "socket.h"
#include "user_consol_interface.h"

static int slave_socket = 0;
static int master_socket = 0;
static const int BUFFER_SIZE_ = 1024;

//обработчик прерывания при закрытии программы через ctrl+c
void term_handler(int i){
	CloseSocket(master_socket);
	CloseSocket(slave_socket);
	printf ("\nTerminating\n");
	
	exit(EXIT_SUCCESS);
}

int main (int argc, char* argv[]){
using namespace std::literals;
	
	//Обработка параметов переданных в программу
	UserRequest ur(argc,argv);

	struct sigaction sa;
	sa.sa_handler = term_handler;  // указываем обработчик
	sigaction(SIGINT, &sa, 0);    // обрабатываем сигнал SIGTERM
    
    //Открытие сокета в программе
	master_socket = OpenSocket (ur.GetIP().data(), ur.GetPORT());

	while ( true )
	{
		//возвращает номер дескриптора, под которым зарегистрирован сокет в ОС
		slave_socket = accept (master_socket, 0, 0 );

		char packet_data[BUFFER_SIZE_];
		
		int resived_bytes = recv(slave_socket, packet_data, sizeof(packet_data), MSG_NOSIGNAL);
		
		//Вывод полученных данных в stdout
		ur.ShowData(packet_data,resived_bytes);
		
		//Answer
		Request request(slave_socket);
		request.Answer(packet_data);
		
		//закрываем соединение
		CloseSocket(slave_socket);
	}
}
