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

void ShowData(const char* packet_data, int resived_bytes){
	std::cout<<"recived "<<resived_bytes<< " bytes"<<std::endl;
	for(int i = 0; i < resived_bytes-1; ++i){
		std::cout<<packet_data[i];
	}
	std::cout<<std::endl;
}

void PrintHelp(std::ostream &stream = std::cerr) {
	std::string help =
R"123(
ways to start the server:
-without parameters: the server will start at 127.0.0.1:12345

-with ip address and port separated by a space "127.0.0.1 12345"
 
-with ip address and port colon space "127.0.0.1:12345"
)123";
	stream << help;
}

std::string ParceIP(const std::string& str){
	std::string result;
	int pos=0;
	while(pos < str.size() && str[pos]!=':'){
		result+=str[pos++];
	}
	return result;
}

int ParcePort(const std::string& str){
	int pos = ParceIP(str).size();
	pos = pos==str.size()? 0 : pos+1;
	return std::stoi(str.substr(pos,str.size()-pos));
}

int main (int argc, char* argv[]){
using namespace std::literals;
std::string IP="127.0.0.1";
int PORT=12345;

	//обработка входных аргументов
	if (argc == 2) {
		const std::string mode(argv[1]);
		if (mode == "-help"s || mode == "-h"s) {
			PrintHelp();
			exit(EXIT_SUCCESS);
		} else {
			try{
				IP = ParceIP(mode);
				PORT = ParcePort(mode);
			}
			catch(...){
				std::cout<<"error_wrong IP"<<std::endl;
				exit(EXIT_SUCCESS);
			}
		} 
	}else if (argc==3){
		try{
			IP = argv[1];
			PORT = ParcePort(argv[2]);
		}
		catch(...){
			std::cout<<"error_wrong IP"<<std::endl;
			exit(EXIT_SUCCESS);
		}
	}

	struct sigaction sa;
	sa.sa_handler = term_handler;  // указываем обработчик
	sigaction(SIGINT, &sa, 0);    // обрабатываем сигнал SIGTERM
    
	master_socket = OpenSocket (IP.data(), PORT);

	while ( true )
	{
		//возвращает номер дескриптора, под которым зарегистрирован сокет в ОС
		slave_socket = accept (master_socket, 0, 0 );

		char packet_data[BUFFER_SIZE_];
		
		int resived_bytes = recv(slave_socket, packet_data, sizeof(packet_data), MSG_NOSIGNAL);
		
		//Вывод полученных данных в stdout
		ShowData(packet_data,resived_bytes);
		
		//Answer
		request::Answer(slave_socket,packet_data);
		
		//закрываем соединение
		CloseSocket(slave_socket);
	}
}
