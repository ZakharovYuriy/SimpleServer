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

//для выдачи страницы
#include <fstream>
#include <filesystem>
#include <string>

static int slave_socket = 0;
static int master_socket = 0;

//посылка страницы
void SendFile(int descriptor, const std::string& file_name){
using namespace std::literals;
using std::filesystem::path;
	path p = path("..") / path("WebPage") / path(file_name);
	std::ifstream file(p, std::ios::in | std::ios::binary);
	if(!file.is_open()){
		std::cout<<"error read file "<<p<<std::endl;
		std::cout<<"file |"<<file_name<<"|"<<std::endl;
		return;
	}
	
	std::string html_ok = "HTTP/1.1 200 OK\n\n"s;
	send (descriptor, html_ok.data(),html_ok.size(), MSG_NOSIGNAL); 
	std::cout<<html_ok;
	
	char buffer[1024];
	int parts = 0;
	do {
		file.read(buffer, sizeof(buffer));
		
			//вывод буффера в консоль
			std::cout<<"Send "<<parts++<<" part"<<std::endl;
			for(int i = 0; i < file.gcount(); ++i){
				std::cout<<buffer[i];
			}
			std::cout<<std::endl;
			
		//отправка ответа
		send (descriptor, buffer,file.gcount(), MSG_NOSIGNAL); 
		
	} while (file);
	
	file.close();  
}

//ответ на запрос
void Answer(unsigned char* packet_data){
using namespace std::literals;
	std::string name;
	int pos=5;
	char temp=packet_data[pos];
	while (temp!=' '){
		name+=temp;
		temp=packet_data[++pos];
	}
	std::cout<<"FileName "<<name<<std::endl;
	if(name.empty()){
		SendFile(slave_socket,"site.html"s);
	}else{
		SendFile(slave_socket,name);
	}
}

//обработчик прерывания при закрытии программы через ctrl+c
void term_handler(int i){
	shutdown(master_socket, SHUT_RDWR);
	close( master_socket );
	
	printf ("Terminating\n");
	
	exit(EXIT_SUCCESS);
}

//установка не блокирующего режима
int SetNonblock (int descriptor){
	int flags;
	#if defined (O_NONBLOCK)
		if(-1==(flags=fcntl(descriptor,F_GETFL,0)))
			flags=0;
		return fcntl(descriptor,F_SETFL,flags|O_NONBLOCK);
	#else
		flags=1;
		return ioctl(descriptor,FIOBIO,&flags);
	#endif
}

int main (){
	struct sigaction sa;
	sa.sa_handler = term_handler;  // указываем обработчик
	sigaction(SIGINT, &sa, 0);    // обрабатываем сигнал SIGTERM
    
	//ipv4, TSP, protocol
	master_socket = socket( AF_INET, SOCK_STREAM, IPPROTO_TCP );

	sockaddr_in address;//IPv4
		address.sin_family = AF_INET;
		address.sin_addr.s_addr = inet_addr("127.0.0.1");//htonl(INADDR_ANY);// IP
		address.sin_port = htons( (unsigned short) 12345 );
		
	//Позволяет переиспользовать адрес поссле закрытия
	int optval = 1;
	setsockopt(master_socket,SOL_SOCKET,SO_REUSEADDR,&optval,sizeof(optval));
	
	if( bind( master_socket, (sockaddr*) &address, sizeof(address)) != 0){
		printf( "failed to bind socket\n" );
		return false;
	}
	
	listen( master_socket, SOMAXCONN );

	while ( true )
	{
		slave_socket = accept (master_socket, 0, 0 );
		
		//при поступлении данных не совпадающей длинны - выпадает ошибка
		//SetNonblock(slave_socket);//перевод в неблокирующий режим

		unsigned char packet_data[1024];
		unsigned int maximum_packet_size = sizeof( packet_data );
		
		//***uncoment whith SetNonblock
		//int resived_bytes=-1;
		//while(resived_bytes<0){
		int resived_bytes =  recv(slave_socket, packet_data, sizeof(packet_data), MSG_NOSIGNAL);
		//}
		
		std::cout<<"recived "<<resived_bytes<< " bytes"<<std::endl;
		for(int i = 0; i < resived_bytes-1; ++i){
			std::cout<<packet_data[i];
		}
		std::cout<<std::endl;
		
		//Answer
		Answer(packet_data);

		shutdown(slave_socket, SHUT_RDWR);
		close( slave_socket );
	}
}
