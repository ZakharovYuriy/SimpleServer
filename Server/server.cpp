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

#include "request_processing.h"
#include "socket.h"

static int slave_socket = 0;
static int master_socket = 0;

//обработчик прерывания при закрытии программы через ctrl+c
void term_handler(int i){
	shutdown(master_socket, SHUT_RDWR);
	close( master_socket );
	
	printf ("\nTerminating\n");
	
	exit(EXIT_SUCCESS);
}

int main (){
using namespace std::literals;
	struct sigaction sa;
	sa.sa_handler = term_handler;  // указываем обработчик
	sigaction(SIGINT, &sa, 0);    // обрабатываем сигнал SIGTERM
    
	master_socket = OpenSocket ("127.0.0.1", 12345);

	while ( true )
	{
		slave_socket = accept (master_socket, 0, 0 );

		unsigned char packet_data[1024];
		unsigned int maximum_packet_size = sizeof( packet_data );
		
		int resived_bytes =  recv(slave_socket, packet_data, sizeof(packet_data), MSG_NOSIGNAL);

		std::cout<<"recived "<<resived_bytes<< " bytes"<<std::endl;
		for(int i = 0; i < resived_bytes-1; ++i){
			std::cout<<packet_data[i];
		}
		std::cout<<std::endl;
		
		//Answer
		Answer(slave_socket,packet_data);

		shutdown(slave_socket, SHUT_RDWR);
		close( slave_socket );
	}
}
