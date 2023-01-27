#pragma once
#include <sys/socket.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <unistd.h>
#include <arpa/inet.h>


int OpenSocket (const char* ip, int port){
	//ipv4, TSP, protocol
	int master_socket = socket( AF_INET, SOCK_STREAM, IPPROTO_TCP );

	sockaddr_in address;
	address.sin_family = AF_INET;//IPv4
	address.sin_addr.s_addr = inet_addr(ip);//htonl(INADDR_ANY);// IP
	address.sin_port = htons( (unsigned short) port );
		
	//Позволяет переиспользовать адрес поссле закрытия
	int optval = 1;
	setsockopt(master_socket,SOL_SOCKET,SO_REUSEADDR,&optval,sizeof(optval));
	
	if( bind( master_socket, (sockaddr*) &address, sizeof(address)) != 0){
		printf( "failed to bind socket\n" );
		return false;
	}
	
	listen( master_socket, SOMAXCONN );
return master_socket;
}

void CloseSocket (int descriptor){
	shutdown(descriptor, SHUT_RDWR);
	close( descriptor );
}
